#pragma once

#include "CPUWorkerThread.h"
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <semaphore>

class FCFSSchedulerThread
{
public:
	FCFSSchedulerThread(int numCores);
	~FCFSSchedulerThread();

	int getNumCores() const { return numberOfCores; }

	// Start the scheduler thread and all worker cores
	void start();

	// Stop gracefully
	void stop();

private:
	void run();

	int numberOfCores;
	std::vector<std::shared_ptr<CPUWorkerThread>> cpuCores;

	std::thread schedulerThread;
	std::atomic<bool> running;

	// The max limit is set to 4
	std::counting_semaphore<4> coreSemaphore;
};
