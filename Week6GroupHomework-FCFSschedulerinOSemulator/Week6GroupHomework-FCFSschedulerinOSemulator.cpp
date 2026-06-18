// Week6GroupHomework-FCFSschedulerinOSemulator.cpp
#include <iostream>
#include <memory>
#include "ProcessManager.h"
#include "FCFSSchedulerThread.h"
#include "MultiplexerConsole.h"

int main()
{
	// Initialize 10 dummy processes
	ProcessManager::getInstance().initializeProcesses();

	// Create the scheduler thread managing 4 CPU cores
	auto scheduler = std::make_shared<FCFSSchedulerThread>(4);
	scheduler->start();

	// Start the multiplexer console to handle user input
	MultiplexerConsole console(scheduler);
	console.run();

	return 0;
}
