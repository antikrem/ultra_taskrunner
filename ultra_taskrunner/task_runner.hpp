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
	std::condition_variable cleanupSignaller;

	std::set<Worker*> workers;
	std::queue<Worker*> workerQueue;
	std::set<Worker*> oldWorkers;

	// lololol
	std::queue<Task*> scheduler;

	// If theres a task and a free worker
	// do an allocation
	void AllocateIfPossible() 
	{
		if (workerQueue.size() != 0 && scheduler.size() != 0) {
			//std::cout << workerQueue.size() << " " << scheduler.size() << std::endl;
			auto worker = workerQueue.front();
			workerQueue.pop();

			auto task = scheduler.front();
			scheduler.pop();

			worker->QueueWork(task);
		}
	}

	void NotifyWorkCompletion(Worker* worker)
	{
		std::unique_lock<std::mutex>(lock);
		workerQueue.push(worker);
		AllocateIfPossible();
	}

	void NotifyCleanup(Worker* worker) {
		int i = 0;
		std::unique_lock<std::mutex>(lock);
		i = 1;
		workers.erase(worker);
		i = 2;
		oldWorkers.insert(worker);
		i = 3;
		cleanupSignaller.notify_one();
		i = 4;
	}

	void CleanUpDeadWorkers() {
		for (auto i : oldWorkers)
		{
			delete i;
		}
		oldWorkers.clear();
	}

public:

	TaskRunner() {
		SpawnWorker(); SpawnWorker(); SpawnWorker(); SpawnWorker(); SpawnWorker(); SpawnWorker(); SpawnWorker();
	}

	void SpawnWorker() {
		std::unique_lock<std::mutex>(lock);
		std::cout << "spawned" << std::endl;
		auto worker = new Worker(this);
		workers.insert(worker);
		workerQueue.push(worker);
	}
	
	void Submit(Task* task) 
	{
		std::unique_lock<std::mutex>(lock);
		scheduler.push(task);
		AllocateIfPossible();
	}

	void SubmitTasks(std::vector<Task*> task)
	{
		std::unique_lock<std::mutex>(lock);
		for (auto i : task)
			scheduler.push(i);
		AllocateIfPossible();
	}

	void Stop() {
		std::unique_lock<std::mutex> l(lock);
		while (!workerQueue.empty()) {
			workerQueue.pop();
		}

		auto temp = workers;
		for (auto worker : temp)
		{
			worker->StopWorker();
		}

		cleanupSignaller.wait(
				l,
				[this]() {
					return workers.size() == 0;
				}
			);

		CleanUpDeadWorkers();
	}

};



#endif // !__TASK_RUNNER_HPP__
