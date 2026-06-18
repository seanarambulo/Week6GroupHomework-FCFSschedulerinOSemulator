#include "ProcessManager.h"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "PrintCommand.h"

void ProcessManager::initializeProcesses()
{
	pmSemaphore.acquire();
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 1; i <= 10; ++i)
	{
		std::stringstream ss;
		ss << "process" << std::setw(2) << std::setfill('0') << i;
		int cmds = 100;
		auto p = std::make_shared<Process>(i, ss.str());

		for (int c = 0; c < cmds; ++c)
		{
			std::string toPrint = "Hello world from " + ss.str() + "!";
			p->addCommand(std::make_shared<PrintCommand>(i, toPrint));
		}

		readyQueue.push(p);
	}
	pmSemaphore.release();
}

bool ProcessManager::isReadyQueueEmpty()
{
	pmSemaphore.acquire();
	bool empty = readyQueue.empty();
	pmSemaphore.release();
	return empty;
}

std::shared_ptr<Process> ProcessManager::dequeueReadyProcess()
{
	pmSemaphore.acquire();
	if (!readyQueue.empty())
	{
		auto p = readyQueue.front();
		readyQueue.pop();
		pmSemaphore.release();
		return p;
	}
	pmSemaphore.release();
	return nullptr;
}

void ProcessManager::addRunningProcess(std::shared_ptr<Process> process)
{
	pmSemaphore.acquire();
	process->setState(ProcessState::RUNNING);

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	struct tm buf;
#if defined(_WIN32) || defined(_WIN64)
	localtime_s(&buf, &in_time_t);
#else
	localtime_r(&in_time_t, &buf);
#endif
	std::stringstream timeStr;
	timeStr << std::put_time(&buf, "%m/%d/%Y %I:%M:%S%p");
	process->setStartTime(timeStr.str());

	runningProcesses.push_back(process);
	pmSemaphore.release();
}

void ProcessManager::removeRunningProcess(std::shared_ptr<Process> process)
{
	pmSemaphore.acquire();
	auto it = std::remove(runningProcesses.begin(), runningProcesses.end(), process);
	if (it != runningProcesses.end())
	{
		runningProcesses.erase(it, runningProcesses.end());
	}
	pmSemaphore.release();
}

void ProcessManager::addFinishedProcess(std::shared_ptr<Process> process)
{
	pmSemaphore.acquire();
	process->setState(ProcessState::FINISHED);
	finishedProcesses.push_back(process);
	pmSemaphore.release();
}

int ProcessManager::getFinishedCount()
{
	pmSemaphore.acquire();
	int count = finishedProcesses.size();
	pmSemaphore.release();
	return count;
}

int ProcessManager::getRunningProcessesCount()
{
	pmSemaphore.acquire();
	int count = runningProcesses.size();
	pmSemaphore.release();
	return count;
}

std::string ProcessManager::getProcessList()
{
	pmSemaphore.acquire();
	std::stringstream ss;

	ss << "--------------------------------------------------\n";
	ss << "Running processes:\n";
	for (auto& p : runningProcesses)
	{
		ss << std::left << std::setw(13) << p->getName()
			<< "(" << p->getStartTime() << ")     "
			<< "Core: " << p->getCoreID() << "     "
			<< p->getCommandCounter() << " / " << p->getTotalCommands() << "\n";
	}

	ss << "\nFinished processes:\n";
	for (auto& p : finishedProcesses)
	{
		ss << std::left << std::setw(13) << p->getName()
			<< "(" << p->getStartTime() << ")     "
			<< "Finished     "
			<< p->getTotalCommands() << " / " << p->getTotalCommands() << "\n";
	}
	ss << "--------------------------------------------------\n";

	std::string result = ss.str();
	pmSemaphore.release();
	return result;
}
