#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class Employee
{
public:
	Employee(string);
	int getId() const;
	int getManagerId() const;
	void setManagerId(int managerId);
	const string& getName() const;
	const list<Employee>& getSubordinates() const;
	void addSubordinate(Employee&);

private:
	int id;
	int getUniqueId();
	string name;
	list<Employee> subordinates;
	int managerId;
};

class System
{
	list<Employee> employees;
	unordered_map<int, Employee*> employeeMap;
public:
	System() {}
	void registerEmployee(Employee&);
	void registerManager(int, int);
	void printDetails(int);
	void printDetails(string);
	list<Employee> getSubordinates(int);
	list<Employee> getSubordinates(string);
};

void System::registerEmployee(Employee& employee)
{
	employees.push_back(employee);
	employeeMap[employee.getId()] = &employees.back();
}

void System::registerManager(int empId, int managerId)
{
	if(!employeeMap.count(empId) || !employeeMap.count(managerId))
	{
		cout << "Either Employee or Manager not registered! Please provide correct registered identifiers to continue\n";
		return;
	}

	employeeMap[empId]->setManagerId(managerId);
	employeeMap[managerId]->addSubordinate(*(employeeMap[empId]));
}

void System::printDetails(int empId)
{
	if(employeeMap.find(empId) == employeeMap.end())
	{
		cout << "Employee not registered! Please provide correct Id and retry\n";
		return;
	}

	cout << "Id: "<< empId << "\t";
	cout << "Name: " << employeeMap[empId]->getName() << "\t";
	int magrId = employeeMap[empId]->getManagerId();
	cout << "Manager: " << employeeMap[magrId]->getName() << endl;
}
void System::printDetails(string prefix)
{
	for(Employee employee: employees)
	{
		if(employee.getName().substr(0, prefix.size()) == prefix)
		{
			cout << "Id: " << employee.getId() << "\tName: " << employee.getName() << "\t";
			cout << "Manager: " << employeeMap[employee.getManagerId()]->getName() << endl;
		}
	}
}

list<Employee> System::getSubordinates(int empId)
{
	if(employeeMap.find(empId) == employeeMap.end())
	{
		cout << "Employee not registered! Please provide correct Id and retry\n";
		list<Employee> dummy;
		return dummy;
	}

	return employeeMap[empId]->getSubordinates();
}
list<Employee> System::getSubordinates(string name)
{
	for(Employee employee: employees)
	{
		if(employee.getName() == name)
			return employee.getSubordinates();
	}

	list<Employee> dummy;
	return dummy;
}

Employee::Employee(string name)
{
	this->name = name;
	this->id = getUniqueId();
	this->managerId = 0;
}

int Employee::getUniqueId()
{
	static int empId = 1;
	return empId++;
}

void Employee::addSubordinate(Employee& employee)
{
	subordinates.push_back(employee);
}

int Employee::getId() const {
	return id;
}

int Employee::getManagerId() const {
	return managerId;
}

void Employee::setManagerId(int managerId) {
	this->managerId = managerId;
}

const string& Employee::getName() const {
	return name;
}

const list<Employee>& Employee::getSubordinates() const {
	return subordinates;
}

int main() {
	Employee employee("Achilees");
	Employee employee1("Hector");
	Employee employee2("Paris");
	Employee employee3("Helen");

	System system;
	system.registerEmployee(employee);
	system.registerEmployee(employee1);
	system.registerEmployee(employee2);
	system.registerEmployee(employee3);

	system.registerManager(employee1.getId(), employee.getId());
	system.registerManager(employee2.getId(), employee.getId());
	system.registerManager(employee3.getId(), employee.getId());
	system.printDetails(employee1.getId());
	cout << "********************************************************************\n";

	system.printDetails("He");

	cout << "********************************************************************\n";

	for(Employee e: system.getSubordinates(employee.getName()))
	{
		cout << e.getName() << " " << e.getId() << endl;
	}

	cout << "********************************************************************\n";

	for(Employee e: system.getSubordinates(employee.getId()))
	{
		cout << e.getName() << " " << e.getId() << endl;
	}

	cout << "********************************************************************\n";

	for(Employee e: system.getSubordinates(employee1.getId()))
	{
		cout << e.getName() << " " << e.getId() << endl;
	}
	return 0;
}
















