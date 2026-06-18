#pragma once
#include <string>
#include "ProcessLogger.h"

class ICommand
{
public:
	enum CommandType { IO, PRINT, DECLARE };

	ICommand(int pid, CommandType type) : pid(pid), commandType(type)
	{
	}

	virtual ~ICommand() = default;

	CommandType getCommandType() const { return commandType; }

	// Virtual execution method with needed context from the Process
	virtual void execute(int coreID, const std::string& processName, ProcessLogger& logFile) = 0;

protected:
	int pid;
	CommandType commandType;
};
