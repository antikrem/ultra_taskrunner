#ifndef __TASK_HPP__
#define __TASK_HPP__

class TaskRunner;

class Task
{

protected:

	virtual void DoTask() = 0;

public:
	
	void Execute();
	
};


#endif // !__TASK_HPP__
