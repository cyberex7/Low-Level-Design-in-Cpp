#include <iostream>
#include <vector>
using namespace std;

class Task;

enum TaskType {STORY, FEATURE, BUG};
enum TaskStatus {OPEN, IN_PROGRESS, RESOLVED, DELAYED, COMPLETED };

class Sprint
{
public:
	Sprint(int begin, int end, string name);
	void addTask(Task&);
	void printDetails();
	bool operator==(Sprint sprint);
	const vector<Task>& getTasks() const;
	void eraseTask(int taskNumber);

private:
	int begin, end;
	string name;
	vector<Task> tasks;
};

class User
{
private:
	vector<Task> taskList;
	vector<Sprint> sprintList;
public:
	Task createTask(TaskType);
	Task createTask(string);
	Sprint createSprint(int, int, string);
	bool addToSprint(Sprint&, Task&);
	bool removeFromSprint(Sprint&, Task&);
	void printAllTasks();
	bool changeStatus(Task&, TaskStatus);
};

class Task
{
public:
	Task();
	int getId() const;
	void setSubtract(const string &subtract);
	void setTaskStatus(TaskStatus taskStatus);
	void setTaskType(TaskType taskType);
	void setUser(User user);

private:
	int id;
	string subtract;
	User user;
	TaskType taskType;
	TaskStatus taskStatus;
	int getUniqueId();
};

Sprint::Sprint(int begin, int end, string name)
{
	this->begin = begin;
	this->end = end;
	this->name = name;
}

void Sprint::addTask(Task& task)
{
	tasks.push_back(task);
}

void Sprint::printDetails()
{
	cout << "Sprint Name: " << this->name << "\nSprint Begins: " << this->begin << "\nSprint Ends: " << this->end << endl;
}

const vector<Task>& Sprint::getTasks() const {
	return tasks;
}

inline bool Sprint::operator==(Sprint sprint)
{
	return (this->begin == sprint.begin) && (this->end == sprint.end) && (this->name == sprint.name);
}

void Sprint::eraseTask(int taskNumber)
{
	tasks.erase(tasks.begin() + taskNumber);
}

Task::Task()
{
	this->id = getUniqueId();
	this->taskStatus = TaskStatus::OPEN;
}

int Task::getId() const {
	return id;
}

void Task::setSubtract(const string &subtract) {
	this->subtract = subtract;
}

void Task::setTaskStatus(TaskStatus taskStatus) {
	this->taskStatus = taskStatus;
}

void Task::setTaskType(TaskType taskType) {
	this->taskType = taskType;
}

void Task::setUser(User user) {
	this->user = user;
}
int Task::getUniqueId()
{
	static int taskId = 1;
	return taskId++;
}

Task User::createTask(TaskType taskType)
{
	if(taskType == TaskType::STORY)
	{
		cout << "Warning! Task of type STORY is being created with no subtract." << endl;
	}
	Task task;
	task.setTaskType(taskType);
	task.setUser(*this);
	taskList.push_back(task);

	return task;
}

Task User::createTask(string subtract)
{

	Task task;
	task.setTaskType(TaskType::STORY);
	task.setSubtract(subtract);
	task.setUser(*this);
	taskList.push_back(task);

	return task;
}

Sprint User::createSprint(int begin, int end, string name)
{
	Sprint sprint(begin, end, name);
	sprintList.push_back(sprint);

	return sprint;
}

bool User::addToSprint(Sprint& sprint, Task& task)
{
	for(Sprint spr: sprintList)
	{
		if(spr == sprint)
		{
			sprint.addTask(task);
			return true;
		}
	}
	return false;
}

bool User::removeFromSprint(Sprint& sprint, Task& task)
{
	vector<Task> taskList;
	for(Sprint spr: sprintList)
	{
		if(spr == sprint)
		{
			taskList = sprint.getTasks();
			break;
		}
	}
	int i = 0;
	for(Task t: taskList)
	{
		if(t.getId() == task.getId())
		{
			sprint.eraseTask(i);
			return true;
		}
		i++;
	}
	return false;
}

void User::printAllTasks()
{
	for(Task task: taskList)
	{
		cout << task.getId() << " ";
	}
	cout << endl;
}

bool User::changeStatus(Task& task, TaskStatus status)
{
	for(Task &t: taskList)
	{
		if(t.getId() == task.getId())
		{
			t.setTaskStatus(status);
			return true;
		}
	}
	return false;
}

int main() {
	User user1, user2;
	Task task1 = user1.createTask(TaskType::FEATURE);
	Task task11 = user1.createTask(TaskType::BUG);
	Task task2 = user2.createTask(TaskType::BUG);
	Task task22 = user2.createTask("This is a subtract");

	Sprint sprint1 = user1.createSprint(22,33,"Sprint1");
	Sprint sprint2 = user2.createSprint(44,55,"Sprint2");

	cout << user1.changeStatus(task11, TaskStatus::IN_PROGRESS) <<endl ;//1

	cout << user1.addToSprint(sprint1, task1) << "\n"; //1
	cout << user1.addToSprint(sprint1, task11) << "\n"; //1
	cout << user1.addToSprint(sprint2, task1) << "\n"; //0
	cout << user1.removeFromSprint(sprint1, task11) << "\n"; //1
	cout << user2.addToSprint(sprint1, task1) << "\n"; //0
	cout << user2.removeFromSprint(sprint1, task2) << "\n"; //0
	cout << user2.addToSprint(sprint2, task1) << "\n"; //1
	cout << user2.addToSprint(sprint2, task2) << "\n"; //1

	sprint1.printDetails();
	cout << endl;
	user1.printAllTasks();
	user2.printAllTasks();
	return 0;
}
