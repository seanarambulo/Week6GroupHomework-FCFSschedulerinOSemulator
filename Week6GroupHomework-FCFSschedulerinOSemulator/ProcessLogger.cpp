#include "ProcessLogger.h"
#include <sstream>
#include <chrono>
#include <iomanip>

ProcessLogger::ProcessLogger(const std::string& processName) : processName(processName)
{
	std::stringstream filename;
	filename << processName << ".txt";
	logFile.open(filename.str(), std::ios::out | std::ios::trunc);
	if (logFile.is_open())
	{
		logFile << "Process name: " << processName << "\n";
		logFile << "Logs:\n\n";
	}
}

ProcessLogger::~ProcessLogger()
{
	if (logFile.is_open())
	{
		logFile.close();
	}
}

void ProcessLogger::log(int coreID, const std::string& message)
{
	if (!logFile.is_open()) return;

	// Format timestamp
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	struct tm buf;
#if defined(_WIN32) || defined(_WIN64)
	localtime_s(&buf, &in_time_t);
#else
	localtime_r(&in_time_t, &buf);
#endif

	logFile << "(" << std::put_time(&buf, "%m/%d/%Y %I:%M:%S%p") << ") "
		<< "Core:" << coreID << " "
		<< "\"" << message << "\"" << std::endl;
}
