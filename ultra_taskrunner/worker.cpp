#include "worker.hpp"

#include "task_runner.hpp"
#include <iostream>

void Worker::DoTaskSpin()
{
	Task* task = nullptr;

	while (workloads.Get(&task)) {
		task->Execute();
		task = nullptr;
		runner->NotifyWorkCompletion(this);
	}

	runner->NotifyCleanup(this);
}

Worker::~Worker() {
	thread.join();
}

Worker::Worker(TaskRunner* runner)
	:thread(&Worker::DoTaskSpin, this)
{
	this->runner = runner;
}

void Worker::QueueWork(Task* task) {
	workloads.Add(task);
}

void Worker::StopWorker() {
	workloads.Stop();
}