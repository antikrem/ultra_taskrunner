#ifndef __ATOMIC_QUEUE_HPP__
#define __ATOMIC_QUEUE_HPP__

#include <queue>
#include <mutex>
#include <iostream>

/**
 * A queue that has only atomic operations
 */
template <class T>
class AtomicQueue 
{
private:

	// Set to true when this queue will be designated to no longer take 
	// inputs 
	bool stopFlag = false;

	std::mutex lock;

	// Notified upon state update (queue is added to or closed)
	std::condition_variable cv;

	std::queue<T> internalQueue;

public:

	// Adds a new element and notifies
	// All waiting get
	void Add(const T& element) 
	{
		std::unique_lock<std::mutex>(lock);
		internalQueue.push(element);
		cv.notify_one();
	}

	// Returns if this queue is empty and stopped
	bool Get(T* element) 
	{
		std::unique_lock<std::mutex> l(lock);

		// Check for a new element or stop flag
		cv.wait(
			l,
			[&]() {
				return stopFlag || internalQueue.size();
			}
		);

		if (internalQueue.size() && !stopFlag) {
			*element = internalQueue.front();
			internalQueue.pop();
			return true;
		}
		else {
			return false;
		}
	}

	// Stop this queue
	// This stops all future adds
	// And makes future gets not wait
	// On empty queue
	void Stop() 
	{
		std::unique_lock<std::mutex>(lock);
		stopFlag = true;
		cv.notify_all();
	}

	int Size() const
	{
		std::unique_lock<std::mutex>(lock);
		return internalQueue.size();
	}

};

#endif // !__ATOMIC_QUEUE_HPP__