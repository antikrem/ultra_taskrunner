#ifndef __WORKER_HPP__
#define __WORKER_HPP__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "atomic_queue.hpp"
#include "task.hpp"

class TaskRunner;


class Worker {

private:

	TaskRunner* runner;

	AtomicQueue<Task*> workloads;
	std::thread thread;
	
	void DoTaskSpin();

public:

	~Worker();

	Worker(TaskRunner* runner);

	void QueueWork(Task* task);

	void StopWorker();

};

#endif // !__WORKER_HPP__