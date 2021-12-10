#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Notepad
{
	vector<string> allContent;
	stack<vector<string>> undoStack;
	stack<vector<string>> redoStack;
	vector<string> split(string, char);
	vector<string> buffer;
public:
	Notepad(string);
	void display();
	bool display(int, int);
	bool insert(int, string);
	bool Delete(int);
	bool Delete(int, int);
	bool copy(int, int);
	bool paste(int);
	bool undo();
	bool redo();
};

Notepad::Notepad(string text)
{
	allContent = split(text, '\n');
}

vector<string> Notepad::split(string text, char delim)
{
	vector<string> result;
	auto pos = text.find(delim);
	auto i = 0;

	while(pos != string::npos)
	{
		result.push_back(text.substr(i, pos-i));
		i = ++pos;
		pos = text.find(delim, pos);
	}

	result.push_back(text.substr(i, text.length()));
	return result;
}

void Notepad::display()
{
	for(string s: allContent)
		cout << s << "\n";
}
bool Notepad::display(int n, int m)
{
	if (static_cast<size_t>(n) > allContent.size())
	{
		cout << "The value of n exceeds number of lines in the file\n";
		return false;
	}
	if (static_cast<size_t>(m) > allContent.size())
	{
		cout << "The value of m exceeds number of lines in the file\n";
		return false;
	}
	if (n > m)
	{
		cout << "The value of n exceeds the value of m\n";
		return false;
	}

	for(vector<string>::iterator iter = allContent.begin() + n - 1; iter != allContent.begin() + m; ++iter)
	{
		cout << *iter << endl;
	}
	return true;
}

bool Notepad::insert(int n, string text)
{
	if (static_cast<size_t>(n) > allContent.size())
	{
		cout << "The value of n exceeds number of lines in the file\n";
		return false;
	}

	undoStack.push(allContent);
	string *content = &(*(allContent.begin() + n - 1));
	*content += text;
	return true;
}

bool Notepad::Delete(int n)
{
	if (static_cast<size_t>(n) > allContent.size())
	{
		cout << "The value of n exceeds number of lines in the file\n";
		return false;
	}

	undoStack.push(allContent);
	allContent.erase(allContent.begin() + n - 1);
	return true;
}
bool Notepad::Delete(int n, int m)
{
	if (static_cast<size_t>(n) > allContent.size())
	{
		cout << "The value of n exceeds number of lines in the file\n";
		return false;
	}
	if (static_cast<size_t>(m) > allContent.size())
	{
		cout << "The value of m exceeds number of lines in the file\n";
		return false;
	}
	if (n > m)
	{
		cout << "The value of n exceeds the value of m\n";
		return false;
	}

	undoStack.push(allContent);
	allContent.erase(allContent.begin() + n - 1, allContent.begin() + m);
	return true;
}

bool Notepad::copy(int n, int m)
{
	if (static_cast<size_t>(n) > allContent.size())
	{
		cout << "The value of n exceeds number of lines in the file\n";
		return false;
	}
	if (static_cast<size_t>(m) > allContent.size())
	{
		cout << "The value of m exceeds number of lines in the file\n";
		return false;
	}
	if (n > m)
	{
		cout << "The value of n exceeds the value of m\n";
		return false;
	}

	for(vector<string>::iterator iter = allContent.begin() + n - 1; iter != allContent.begin() + m; ++iter)
	{
		buffer.push_back(*iter);
	}
	return true;
}

bool Notepad::paste(int n)
{
	if (static_cast<size_t>(n) > allContent.size())
	{
		cout << "The value of n exceeds number of lines in the file\n";
		return false;
	}

	undoStack.push(allContent);
	allContent.insert(allContent.begin() + n - 1, buffer.begin(), buffer.end());
	return true;
}

bool Notepad::undo()
{
	if(undoStack.empty())
	{
		cout << "Nothing to undo!\n";
		return false;
	}

	redoStack.push(allContent);
	allContent = undoStack.top();
	undoStack.pop();

	return true;
}
bool Notepad::redo()
{
	if(redoStack.empty())
	{
		cout << "Nothing to redo!\n";
		return false;
	}

	undoStack.push(allContent);
	allContent = redoStack.top();
	redoStack.pop();

	return true;
}

int main() {
	Notepad notepad("At the starting of the week\nAt summit talks you'll hear them speak\nIt's only Monday");
	notepad.display();
	cout << "**************************** 0 ***********************************\n";
	cout << "** Displaying content: only first two lines **\n";
	notepad.display(1,2);
	cout << "**************************** 1 ***********************************\n";
	cout << "** Inserting yeah to the first line **\n";
	notepad.insert(1, ", Yeah");
	notepad.display();
	cout << "**************************** 2 ***********************************\n";
	cout << "** Undoing last move **\n";
	notepad.undo();
	notepad.display();
	cout << "***************************** 3 *********************************\n";
	cout << "** Redoing last move **\n";
	notepad.redo();
	notepad.display();
	cout << "****************************** 4 *******************************\n";
	cout << "** Redoing last move **\n";
	notepad.redo();
	cout << "******************************** 5 ****************************\n";
	cout <<"** Deleting first line **\n";
	notepad.Delete(1);
	notepad.display();
	cout << "******************************* 6 ****************************\n";
	cout << "** Undoing last move **\n";
	notepad.undo();
	notepad.display();
	cout << "******************************* 7 ***************************\n";
	cout << "** Undoing last move **\n";
	notepad.undo();
	notepad.display();
	cout << "**************************** 8 ***********************************\n";
	cout << "** After deletion of lines 1 to 2 **\n";
	notepad.Delete(1,2);
	notepad.display();
	cout << "***************************** 9 ****************************\n";
	cout << "** Undoing last move **\n";
	notepad.undo();
	notepad.display();
	cout << "***************************** 10 ***************************\n";
	cout << "** Copying lines 1 to 2 and pasting them on 3rd line **\n";
	notepad.copy(1,2);
	notepad.paste(3);
	notepad.display();
	cout << "***************************** 11 **************************\n";
	cout << "** Undoing last move **\n";
	notepad.undo();
	notepad.display();
	cout << "****************************** 12 ************************\n";
	cout << "** Redoing last move **\n";
	notepad.redo();
	notepad.display();
	return 0;
}
