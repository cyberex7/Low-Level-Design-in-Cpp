#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <list>
using namespace std;

enum Split {EQUAL, EXACT, PERCENT};

class User
{
public:
	User(string &);
	int getId() const;
	const string& getName() const;
	double getTotalExpenseSoFar() const;
	const vector<pair<User, double> >& getUserExpenseSheet() const;
	void addToUserExpenseSheet(User &, double);
	bool operator==(const User& user) const;
	void printTotalBalance();

private:
	int id;
	string name;
	vector<pair<User, double>> userExpenseSheet;
	double totalExpenseSoFar;
	int getUniqueId();
};

class Expense
{
public:
	Expense(User&, Split, vector<User>&, double);
	const string& getDescription() const;
	void setDescription(const string &description);
	const vector<double>& getExactDistribution() const;
	void setExactDistribution(const vector<double> &exactDistribution);
	int getId() const;
	const vector<double>& getPercentDistribution() const;
	void setPercentDistribution(const vector<double> &perentDistribution);
	Split getSplit() const;
	const User* getCreditor() const;
	const vector<User>& getDefaulters() const;
	double getExactTotalAmount() const;

private:
	int id;
	string description;
	Split split;
	vector<double> percentDistribution;
	vector<double> exactDistribution;
	User *creditor;
	vector<User> defaulters;
	double exactTotalAmount;

	int getUniqueId();
};

class Splitwise
{
public:
	Splitwise() {}
	void registerUser(User&);
	void addExpense(Expense&);
	void printBalanceForAllUsers();
	const list<User>& getUsers() const;
	void simplifyExpenses();

private:
	list<User> users;
	unordered_map<int, User*> userIdMap;
	bool calculateExpenses(Expense&);
	bool verifyUsers(User, vector<User>);
	vector<double> divideEqually(double, size_t);
};

User::User(string &name)
{
	this->name = name;
	this->id = getUniqueId();
	this->totalExpenseSoFar = 0;
	this->userExpenseSheet.clear();
}

void User::addToUserExpenseSheet(User& user, double value)
{
	if(*this == user)
		return;

	this->totalExpenseSoFar += value;
	for(pair<User, double> &newExpense: userExpenseSheet)
	{
		if(newExpense.first == user)
		{
			newExpense.second += value;
			return;
		}
	}

	userExpenseSheet.push_back(make_pair(user, value));
}

void User::printTotalBalance()
{
	if(totalExpenseSoFar > 0)
        cout << this->getName().c_str() << " owes a total of " << this->totalExpenseSoFar << "\n";
	else
        cout << this->getName().c_str() << " gets back a total of " << this->totalExpenseSoFar * (-1) << "\n";
}

inline bool User::operator==(const User& user) const
{
	return this->id == user.getId();
}

int User::getUniqueId()
{
	static int userId = 1;
	return userId++;
}

int User::getId() const {
	return id;
}

const string& User::getName() const {
	return name;
}

double User::getTotalExpenseSoFar() const {
	return totalExpenseSoFar;
}

const vector<pair<User, double> >& User::getUserExpenseSheet() const {
	return userExpenseSheet;
}

Expense::Expense(User& creditor, Split split, vector<User>& defaulters, double exactTotalAmount)
{
	this->creditor = &creditor;
	this->split = split;
	this->defaulters = defaulters;
	this->exactTotalAmount = exactTotalAmount;
	this->id = getUniqueId();
}

const string& Expense::getDescription() const {
	return description;
}

void Expense::setDescription(const string &description) {
	this->description = description;
}

const vector<double>& Expense::getExactDistribution() const {
	return exactDistribution;
}

void Expense::setExactDistribution(const vector<double> &exactDistribution) {
	this->exactDistribution = exactDistribution;
}

int Expense::getId() const {
	return id;
}

const vector<double>& Expense::getPercentDistribution() const {
	return percentDistribution;
}

void Expense::setPercentDistribution(const vector<double> &percentDistribution) {
	this->percentDistribution = percentDistribution;
}

Split Expense::getSplit() const {
	return split;
}

const User* Expense::getCreditor() const {
	return creditor;
}

const vector<User>& Expense::getDefaulters() const {
	return defaulters;
}

double Expense::getExactTotalAmount() const {
	return exactTotalAmount;
}

int Expense::getUniqueId()
{
	static int expenseId = 1;
	return expenseId++;
}

void Splitwise::registerUser(User &user)
{
	if(userIdMap.find(user.getId()) == userIdMap.end())
	{
		users.push_back(user);
		userIdMap[user.getId()] = &users.back();
	}
}

void Splitwise::addExpense(Expense& expense)
{
	if(verifyUsers(*expense.getCreditor(), expense.getDefaulters()) == false)
	{
		cout << "Can't process expense. Kindly register all users and retry\n";
		return;
	}
	calculateExpenses(expense);
}

bool Splitwise::verifyUsers(User user, vector<User> users)
{
	if(find(users.begin(), users.end(), user) == users.end())
		users.push_back(user);

	for(User usr: users)
		if(userIdMap.find(usr.getId()) == userIdMap.end())
			return false;
	return true;
}

bool Splitwise::calculateExpenses(Expense& expense)
{
	User creditor = *expense.getCreditor();
	vector<User> defaulters = expense.getDefaulters();
	vector<double> amtPerHead;
	switch(expense.getSplit())
	{
	case Split::EQUAL:
		amtPerHead = divideEqually(expense.getExactTotalAmount(), defaulters.size());
		for(size_t i=0; i < defaulters.size(); ++i)
		{
			userIdMap[creditor.getId()]->addToUserExpenseSheet(defaulters[i], (-1) * amtPerHead[i]);
			userIdMap[defaulters[i].getId()]->addToUserExpenseSheet(creditor, amtPerHead[i]);
		}
		break;
	case Split::EXACT:
		amtPerHead = expense.getExactDistribution();
		if(expense.getExactTotalAmount() != accumulate(amtPerHead.begin(), amtPerHead.end(), 0))
		{
			cout << "Can't create expense. Total amount doesn't equal sum of individual amounts. "
	                "Please try again!\n";
			return false;
		}

		if(amtPerHead.size() != defaulters.size())
		{
			cout << "The amounts and value numbers don't match. Expense can't be created. "
			                "Please try again!\n";
			return false;
		}
		for(size_t i=0; i < defaulters.size(); ++i)
		{
			userIdMap[creditor.getId()]->addToUserExpenseSheet(defaulters[i], (-1) * amtPerHead[i]);
			userIdMap[defaulters[i].getId()]->addToUserExpenseSheet(creditor, amtPerHead[i]);
		}
		break;
	case Split::PERCENT:
		amtPerHead = expense.getPercentDistribution();
		if(100 != accumulate(amtPerHead.begin(), amtPerHead.end(), 0))
		{
			cout << "Can't create expense. All percentages don't add to 100. "
	                "Please try again!\n";
			return false;
		}

		if(amtPerHead.size() != defaulters.size())
		{
			cout << "The percents and value numbers don't match. Expense can't be created. "
			                "Please try again!\n";
			return false;
		}
		for(size_t i=0; i < defaulters.size(); ++i)
		{
			double amount = (amtPerHead[i] * expense.getExactTotalAmount()) / 100.0;
			amount = floor((amount * 100.0) + 0.5) / 100.0;

			userIdMap[creditor.getId()]->addToUserExpenseSheet(defaulters[i], (-1) * amount);
			userIdMap[defaulters[i].getId()]->addToUserExpenseSheet(creditor, amount);
		}
		break;
	default:
		break;
	}
	return true;
}

void Splitwise::printBalanceForAllUsers()
{
	for(User user: users)
		user.printTotalBalance();
}

const list<User>& Splitwise::getUsers() const {
	return users;
}

vector<double> Splitwise::divideEqually(double totalAmount, size_t memberCount)
{
	vector<double> parts;
	for(size_t i = 0; i < memberCount; ++i)
	{
		double part = trunc((100.0 * totalAmount)/(memberCount - i)) / 100.0;
		parts.push_back(part);
		totalAmount -= part;
	}

	return parts;
}
void Splitwise::simplifyExpenses()
{
	vector<int> amounts;
	for(User &user: users)
	{
		amounts.push_back((-1) * user.getTotalExpenseSoFar());
	}

	pair<vector<int>::iterator, vector<int>::iterator> min_max = minmax_element(amounts.begin(), amounts.end());

	while(*min_max.first || *min_max.second)
	{
		int min_index = min_max.first - amounts.begin();
		int max_index = min_max.second - amounts.begin();

		int min_amount = min(-amounts[min_index], amounts[max_index]);

		amounts[min_index] += min_amount;
		amounts[max_index] -= min_amount;

		auto it_min = users.begin();
		auto it_max = users.begin();

		advance(it_min, min_index);
		advance(it_max, max_index);

		cout << it_min->getName() << " pays the amount " << min_amount << " to " << it_max->getName() << endl;

		min_max = minmax_element(amounts.begin(), amounts.end());
	}
}

int main() {
    string s1 = "Jitu";
    string s2 = "Navin";
    string s3 = "Yogi";
    string s4 = "Mandal";
    User u1(s1);
    User u2(s2);
    User u3(s3);
    User u4(s4);

    vector<User> users;
    users.push_back(u1);
    users.push_back(u2);
    users.push_back(u3);
    users.push_back(u4);

    Splitwise sp;
    sp.registerUser(u1);
    sp.registerUser(u2);
    sp.registerUser(u3);
    sp.registerUser(u4);

    Expense expense(u1, Split::EQUAL, users, 2000);
    sp.addExpense(expense);

    sp.printBalanceForAllUsers();

    vector<User> users2;
    users2.push_back(u2);
    users2.push_back(u3);

    Expense expense2(u1, Split::EXACT, users2, 1400);
    sp.addExpense(expense2);

    vector<double> db = {500, 900};
    expense2.setExactDistribution(db);
    sp.addExpense(expense2);
    sp.printBalanceForAllUsers();

    vector<double> db2 = { 40,20,20,20 };
    vector<User> users3 = { u1, u2, u3, u4 };

    Expense expense3(u4, Split::PERCENT, users3, 1200);
    expense3.setPercentDistribution(db2);
    sp.addExpense(expense3);
    sp.printBalanceForAllUsers();
    cout << endl;
    cout << endl;

    for (User user : sp.getUsers())
    {
        for (pair<User, double> p : user.getUserExpenseSheet())
        {
            if (p.second > 0)
                cout << user.getName().c_str() << " owes a total of " << p.second << " to " << p.first.getName().c_str() << "\n";
            else
                cout << user.getName().c_str() << " gets back a total of " << p.second * (-1) << " from " << p.first.getName().c_str() << "\n";

        }
    }

    cout << endl;
    sp.simplifyExpenses();
	return 0;
}
