#include "CPUWorkerThread.h"
#include "ProcessManager.h"
#include <chrono>
#include <iostream>

CPUWorkerThread::CPUWorkerThread(int id) : coreID(id), currentProcess(nullptr), running(false), isAvailable(true)
{
}

CPUWorkerThread::~CPUWorkerThread()
{
	stop();
}

void CPUWorkerThread::start()
{
	running = true;
	workerThread = std::thread(&CPUWorkerThread::run, this);
}

void CPUWorkerThread::stop()
{
	stateMutex.acquire();
	running = false;
	stateMutex.release();
	assignSignal.release();
	if (workerThread.joinable())
	{
		workerThread.join();
	}
}

void CPUWorkerThread::setSchedulerSemaphore(std::counting_semaphore<4>* sem)
{
	schedulerSemaphore = sem;
}

void CPUWorkerThread::assignProcess(std::shared_ptr<Process> p)
{
	stateMutex.acquire();
	currentProcess = p;
	isAvailable = false;
	stateMutex.release();
	assignSignal.release();
}

bool CPUWorkerThread::isIdle()
{
	stateMutex.acquire();
	bool availabilityStatus = isAvailable;
	stateMutex.release();
	return availabilityStatus;
}

void CPUWorkerThread::run()
{
	// While the OS emulator is running:
	while (running)
	{
		std::shared_ptr<Process> pToExecute = nullptr;

		// Wait until a process is assigned or we are asked to stop
		assignSignal.acquire();

		stateMutex.acquire();
		if (!running)
		{
			stateMutex.release();
			break;
		}

		pToExecute = currentProcess;
		stateMutex.release(); // Explicitly unlock before executing the process

		if (pToExecute != nullptr)
		{
			executeProcess(pToExecute);
		}
	}
}

void CPUWorkerThread::executeProcess(std::shared_ptr<Process> pToExecute)
{
	// Assign core ID so it can be displayed correctly
	pToExecute->setCoreID(coreID);

	// Log that we started running the process
	ProcessManager::getInstance().addRunningProcess(pToExecute);

	// Execute all commands to completion (because FCFS is non-preemptive)
	while (!pToExecute->isFinished())
	{
		pToExecute->executeCurrentCommand(coreID);
		pToExecute->moveToNextLine();

		// Simulate some work taking time so it's not instantaneous
		if (pToExecute->getCommandCounter() % 10 == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	// Mark the process as FINISHED
	ProcessManager::getInstance().removeRunningProcess(pToExecute);
	ProcessManager::getInstance().addFinishedProcess(pToExecute);

	// The file stream is closed automatically when Process destructor runs,
	// but we ensure the object state is finalized.

	// Return to an available state
	stateMutex.acquire();
	currentProcess = nullptr;
	isAvailable = true;
	stateMutex.release();

	if (schedulerSemaphore)
	{
		schedulerSemaphore->release();
	}
}
