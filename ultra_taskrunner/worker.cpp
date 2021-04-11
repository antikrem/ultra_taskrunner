#include "worker.hpp"

#include <iostream>

void Worker::DoTaskSpin()
{
	Task* task = nullptr;

	while (workqueue->Get(&task)) {
		task->Execute();
		task = nullptr;
	}
}

Worker::~Worker() {
	thread.join();
}

Worker::Worker(AtomicQueue<Task*>* workqueue)
	: workqueue(workqueue), thread(&Worker::DoTaskSpin, this)
{

}