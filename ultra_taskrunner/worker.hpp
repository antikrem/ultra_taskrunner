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

	AtomicQueue<Task*>* workqueue;

	std::thread thread;


	void DoTaskSpin();

public:

	~Worker();

	Worker(AtomicQueue<Task*>* workqueue);

};

#endif // !__WORKER_HPP__