#include "FCFSSchedulerThread.h"
#include "ProcessManager.h"
#include <chrono>
#include <cassert>

FCFSSchedulerThread::FCFSSchedulerThread(int numCores) : numberOfCores(numCores), running(false),
                                                         coreSemaphore(numCores)
{
	for (int i = 0; i < numberOfCores; ++i)
	{
		auto core = std::make_shared<CPUWorkerThread>(i);
		core->setSchedulerSemaphore(&coreSemaphore);
		cpuCores.push_back(core);
	}
}

FCFSSchedulerThread::~FCFSSchedulerThread()
{
	stop();
}

void FCFSSchedulerThread::start()
{
	running = true;
	for (auto& core : cpuCores)
	{
		core->start();
	}
	schedulerThread = std::thread(&FCFSSchedulerThread::run, this);
}

void FCFSSchedulerThread::stop()
{
	running = false;
	coreSemaphore.release(numberOfCores); // Ensure we unblock
	if (schedulerThread.joinable())
	{
		schedulerThread.join();
	}
	for (auto& core : cpuCores)
	{
		core->stop();
	}
}

void FCFSSchedulerThread::run()
{
	// While the OS emulator is running:
	while (running)
	{
		// If the Ready Queue is empty, poll shortly
		if (ProcessManager::getInstance().isReadyQueueEmpty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}

		// Wait until at least one core is available, blocking to avoid 100% CPU usage
		if (!coreSemaphore.try_acquire_for(std::chrono::milliseconds(50)))
		{
			continue;
		}

		if (!running)
		{
			break;
		}

		// Find an available CPUWorkerThread.
		std::shared_ptr<CPUWorkerThread> idleCore = nullptr;
		for (auto& core : cpuCores)
		{
			if (core->isIdle())
			{
				idleCore = core;
				break;
			}
		}

		// If an idle core is found:
		if (idleCore != nullptr)
		{
			std::shared_ptr<Process> p = ProcessManager::getInstance().dequeueReadyProcess();
			if (p)
			{
				// Assign the process to the idle core
				idleCore->assignProcess(p);
			}
			else
			{
				coreSemaphore.release();
			}
		}
		else
		{
			coreSemaphore.release();
		}
	}
}
