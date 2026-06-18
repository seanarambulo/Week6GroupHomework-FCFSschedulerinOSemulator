#pragma once
#include <string>
#include <fstream>

class ProcessLogger
{
public:
	ProcessLogger(const std::string& processName);
	~ProcessLogger();

	void log(int coreID, const std::string& message);

private:
	std::ofstream logFile;
	std::string processName;
};
