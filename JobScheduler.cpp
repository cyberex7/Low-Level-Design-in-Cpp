#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

enum User {ROOT, ADMIN, USER};
enum SchedulingAlgorithm {FCFS, SJF, FPS, EDF};

class Job
{
public:
	Job(string, int, int, int, User);
	int getDeadline() const;
	int getDuration() const;
	int getId() const;
	const string& getName() const;
	int getPriority() const;
	User getUser() const;

private:
	int id;
	string name;
	int duration;
	int priority;
	int deadline;
	User user;
	int getUniqueId();
};
struct fcfsComparator
{
	bool operator()(const Job& lhs, const Job& rhs) const
	{
		return lhs.getId() > rhs.getId();
	}
};
struct sjfComparator
{
	bool operator()(const Job& lhs, const Job& rhs) const
	{
		return lhs.getDuration() > rhs.getDuration()
				|| (lhs.getDuration() == rhs.getDuration() &&
						lhs.getPriority() > rhs.getPriority());
	}
};
struct fpsComparator
{
	bool operator()(const Job& lhs, const Job& rhs) const
	{
		return lhs.getPriority() > rhs.getPriority()
				|| (lhs.getPriority() == rhs.getPriority() &&
						lhs.getUser() > rhs.getUser())
					|| (lhs.getPriority() == rhs.getPriority() &&
								lhs.getUser() == rhs.getUser() &&
										lhs.getDuration() < rhs.getDuration());
	}
};
struct edfComparator
{
	bool operator()(const Job& lhs, const Job& rhs) const
	{
		return lhs.getDeadline() > rhs.getDeadline()
				|| (lhs.getDeadline() == rhs.getDeadline() &&
						lhs.getPriority() > rhs.getPriority())
					|| (lhs.getDeadline() == rhs.getDeadline() &&
								lhs.getPriority() == rhs.getPriority() &&
										lhs.getDuration() > rhs.getDuration());
	}
};

class Scheduler
{
	vector<Job> jobs;
	void processThreads(vector<int>&);
	priority_queue<Job, vector<Job>, fcfsComparator> fcfsQueue;
	priority_queue<Job, vector<Job>, sjfComparator> sjfQueue;
	priority_queue<Job, vector<Job>, fpsComparator> fpsQueue;
	priority_queue<Job, vector<Job>, edfComparator> edfQueue;
public:
	Scheduler() {}
	void addJob(Job&);
	vector<vector<Job>> getSchedulingSequence(SchedulingAlgorithm, int);
};

Job::Job(string name, int duration, int priority, int deadline, User user)
{
	this->id = getUniqueId();
	this->name = name;
	this->duration = duration;
	this->priority = priority;
	this->deadline = deadline;
	this->user = user;
}

int Job::getUniqueId()
{
	static int jobId = 1;
	return jobId++;
}

int Job::getDeadline() const {
	return deadline;
}

int Job::getDuration() const {
	return duration;
}

int Job::getId() const {
	return id;
}

const string& Job::getName() const {
	return name;
}

int Job::getPriority() const {
	return priority;
}

User Job::getUser() const {
	return user;
}

void Scheduler::addJob(Job& job)
{
	jobs.push_back(job);
	fcfsQueue.push(job);
	sjfQueue.push(job);
	fpsQueue.push(job);
	edfQueue.push(job);
}

void Scheduler::processThreads(vector<int>& threadCapacity)
{
	int minElement = *min_element(threadCapacity.begin(), threadCapacity.end());
	for(size_t i = 0; i < threadCapacity.size(); ++i)
	{
		threadCapacity[i] -= minElement;
	}
}

vector<vector<Job>> Scheduler::getSchedulingSequence(SchedulingAlgorithm algorithm, int threads)
{
	vector<vector<Job>> result;
	result.resize(threads);
	vector<int> threadCapacity(threads, 0);
	vector<int> totalTimeTaken(threads, 0);
	switch(algorithm)
	{
	case SchedulingAlgorithm::FCFS:
		while(fcfsQueue.empty() == false)
		{
			for(int i=0; i< threads; ++i)
			{
				if(threadCapacity[i] == 0)
				{
					if(fcfsQueue.empty() == false)
					{
						Job job = fcfsQueue.top();
						result[i].push_back(job);
						threadCapacity[i] += job.getDuration();
						fcfsQueue.pop();
					}
				}
			}
			processThreads(threadCapacity);
		}
		break;
	case SchedulingAlgorithm::SJF:
		while(sjfQueue.empty() == false)
		{
			for(int i=0; i< threads; ++i)
			{
				if(threadCapacity[i] == 0)
				{
					if(sjfQueue.empty() == false)
					{
						Job job = sjfQueue.top();
						result[i].push_back(job);
						threadCapacity[i] += job.getDuration();
						sjfQueue.pop();
					}
				}
			}
			processThreads(threadCapacity);
		}
		break;
	case SchedulingAlgorithm::FPS:
		while(fpsQueue.empty() == false)
		{
			for(int i=0; i< threads; ++i)
			{
				if(threadCapacity[i] == 0)
				{
					if(fpsQueue.empty() == false)
					{
						Job job = fpsQueue.top();
						result[i].push_back(job);
						threadCapacity[i] += job.getDuration();
						fpsQueue.pop();
					}
				}
			}
			processThreads(threadCapacity);
		}
		break;
	case SchedulingAlgorithm::EDF:
		while(edfQueue.empty() == false)
		{
			for(int i=0; i< threads; ++i)
			{
				if(threadCapacity[i] == 0)
				{
					if(edfQueue.empty() == false)
					{
						Job job = edfQueue.top();
						totalTimeTaken[i] += job.getDuration();
						if(totalTimeTaken[i] <= job.getDeadline())
							result[i].push_back(job);
						else
							totalTimeTaken[i] -= job.getDuration();

						threadCapacity[i] += job.getDuration();
						edfQueue.pop();
					}
				}
			}
			processThreads(threadCapacity);
		}
		break;
	default:
		break;
	}
	return result;
}

int main() {
	Job j1("J1", 10, 0, 10, User::ROOT);
	Job j2("J2", 20, 0, 40, User::ADMIN);
	Job j3("J3", 15, 2, 40, User::ROOT);
	Job j4("J4", 30, 1, 40, User::USER);
	Job j5("J5", 10, 2, 30, User::USER);

	Scheduler scheduler;
	scheduler.addJob(j1);
	scheduler.addJob(j2);
	scheduler.addJob(j3);
	scheduler.addJob(j4);
	scheduler.addJob(j5);

	cout << "************************************ FCFS ***********************************" << endl;

	vector<vector<Job>> res = scheduler.getSchedulingSequence(SchedulingAlgorithm::FCFS, 2);

    for (size_t i = 0; i< res.size(); ++i)
    {
        vector<Job> temp = res[i];
        for (Job job : temp)
            cout << job.getName().c_str() << " ";
        cout << endl;
    }

	cout << "************************************ SJF ************************************" << endl;

	res = scheduler.getSchedulingSequence(SchedulingAlgorithm::SJF, 2);

    for (size_t i = 0; i< res.size(); ++i)
    {
        vector<Job> temp = res[i];
        for (Job job : temp)
            cout << job.getName().c_str() << " ";
        cout << endl;
    }

	cout << "************************************ FPS ************************************" << endl;

	res = scheduler.getSchedulingSequence(SchedulingAlgorithm::FPS, 2);

    for (size_t i = 0; i< res.size(); ++i)
    {
        vector<Job> temp = res[i];
        for (Job job : temp)
            cout << job.getName().c_str() << " ";
        cout << endl;
    }

    cout << "************************************ EDF ************************************" << endl;

	res = scheduler.getSchedulingSequence(SchedulingAlgorithm::EDF, 2);

    for (size_t i = 0; i< res.size(); ++i)
    {
        vector<Job> temp = res[i];
        for (Job job : temp)
            cout << job.getName().c_str() << " ";
        cout << endl;
    }
	return 0;
}
































