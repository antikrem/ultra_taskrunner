// ultra_taskrunner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

#include "task_runner.hpp"
#include "anonymous_task.hpp"

int main()
{
	TaskRunner runner;

	std::string nice = "nice";


	auto tasklast
		= new AnonymousTask(
			[]() {
				std::cout << "done" << std::endl;
			}
	);

	std::vector<Task*> temp;

	for (int i = 0; i < 100; i++) {
		temp.push_back(new AnonymousTask(
			[]() {
				int sum = 0;
				for (int i = 0; i < 10000; i++) {
					sum += i;
				}
			}
		));
	}

	runner.SubmitTasks(temp);
	runner.Submit(tasklast);

	Sleep(200000);

	runner.Stop();

}