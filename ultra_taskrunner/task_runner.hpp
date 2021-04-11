#ifndef __TASK_RUNNER_HPP__
#define __TASK_RUNNER_HPP__

#include <thread>
#include <set>


#include "task.hpp"

#include "worker.hpp"
#include <vector>

#include <iostream>
#include <Windows.h>

class TaskRunner
{

private:
	friend class Worker;

	std::mutex lock;

	std::set<Worker*> workers;

	AtomicQueue<Task*> taskqueue;

public:

	TaskRunner() 
	{
		SpawnWorker(); SpawnWorker();
	}

	void SpawnWorker() {
		std::unique_lock<std::mutex>(lock);
		auto worker = new Worker(&taskqueue);
		workers.insert(worker);
	}
	
	void Submit(Task* task) 
	{
		std::unique_lock<std::mutex>(lock);
		taskqueue.Add(task);
	}

	void SubmitTasks(std::vector<Task*> tasks)
	{
		std::unique_lock<std::mutex>(lock);
		for (auto task : tasks)
			taskqueue.Add(task);
	}

	void Stop() {
		std::unique_lock<std::mutex> l(lock);
		taskqueue.Stop();
	}

};



#endif // !__TASK_RUNNER_HPP__
