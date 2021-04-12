#ifndef __ANONYMOUS_TASK_HPP__
#define __ANONYMOUS_TASK_HPP__


#include <functional>

#include "task.hpp"


class AnonymousTask : public Task 
{
	std::function<void(void)> innerTask;

protected:

	void DoTask() 
	{
		this->innerTask();
	}

public:
	
	AnonymousTask(std::function<void(void)> task) {
		this->innerTask = task;
	}

};


#endif // !__ANONYMOUS_TASK_HPP__