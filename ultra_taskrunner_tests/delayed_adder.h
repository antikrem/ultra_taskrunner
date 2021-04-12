#pragma once

#include "../ultra_taskrunner/atomic_queue.hpp"
#include <windows.h>
#include <chrono>

// Helper to pass a 
template <class T>
class DelayedAdder {

private:

	AtomicQueue<T>* queue = nullptr;

	std::vector<std::thread*> threads;

	std::mutex lock;

	void DelayedAddCallback(int milliseconds, T value)
	{
		Sleep(milliseconds);
		queue->Add(value);
	}

	void DelayedStopCallback(int milliseconds)
	{
		Sleep(milliseconds);
		queue->Stop();
	}

public:

	DelayedAdder(AtomicQueue<T>* queue) 
	{
		this->queue = queue;
	}

	void DelayedAdd(int milliseconds, T value) 
	{
		std::unique_lock<std::mutex>(lock);
		auto thread = new std::thread(&DelayedAdder::DelayedAddCallback, this, milliseconds, value);
		threads.push_back(thread);
	}

	void DelayedStop(int milliseconds)
	{
		std::unique_lock<std::mutex>(lock);
		auto thread = new std::thread(&DelayedAdder::DelayedStopCallback, this, milliseconds);
		threads.push_back(thread);
	}

	std::chrono::system_clock::time_point Tic() 
	{
		return std::chrono::system_clock::now();
	}

	long Toc(std::chrono::system_clock::time_point tic)
	{
		return (long)std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now() - tic
			).count();
	}

	~DelayedAdder()
	{
		std::unique_lock<std::mutex>(lock);
		for (auto& i : threads)
		{
			i->join();
		}
	}


};