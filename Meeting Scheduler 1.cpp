#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Meeting;
typedef unordered_map<int, vector<Meeting>> Calendar;

class Room
{
public:
	Room(string);
	bool book(int, int, int);
	const string& getName() const;

private:
	string name;
	Calendar calendar;
};

class Meeting
{
public:
	Meeting(int, int, Room);
	int getEnd() const;
	int getStart() const;

private:
	int start, end;
	Room room;
};

class Scheduler
{
	vector<Room> rooms;
public:
	Scheduler(vector<Room> rooms);
	string book(int, int, int);
};

Room::Room(string name)
{
	this->name = name;
}

bool Room::book(int day, int start, int end)
{
	for(Meeting m: calendar[day])
	{
		if(m.getStart() < end && start < m.getEnd())
			return false;
	}

	Meeting meeting(start, end, *this);
	calendar[day].push_back(meeting);

	return true;
}

const string& Room::getName() const {
	return name;
}

Meeting::Meeting(int start, int end, Room room): room(room)
{
	this->start = start;
	this->end = end;
}

int Meeting::getEnd() const {
	return end;
}

int Meeting::getStart() const {
	return start;
}

Scheduler::Scheduler(vector<Room> rooms)
{
	this->rooms = rooms;
}

string Scheduler::book(int day, int start, int end)
{
	for(Room &room: rooms)
	{
		bool flag = room.book(day, start, end);
		if(flag == true)
			return room.getName();
	}

	return "No rooms available";
}

int main() {
	Room room1("Atlas");
	Room room2("Nexus");
	Room room3("HolyCow");

	vector<Room> roomVec;
	roomVec.push_back(room1);
	roomVec.push_back(room2);
	roomVec.push_back(room3);

	Scheduler scheduler(roomVec);

	cout << scheduler.book(15,2,5) << "\n";	//Atlas
	cout << scheduler.book(15,5,8) << "\n";	//Atlas
	cout << scheduler.book(15,4,8) << "\n";	//Nexus
	cout << scheduler.book(15,3,6) << "\n";	//Holycow
	cout << scheduler.book(15,7,8) << "\n";	//Holycow
	cout << scheduler.book(16,6,9) << "\n";	//Atlas
	return 0;
}
