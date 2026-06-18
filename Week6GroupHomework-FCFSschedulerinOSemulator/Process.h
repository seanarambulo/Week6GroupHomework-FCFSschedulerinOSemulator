#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include "ICommand.h"
#include "SymbolTable.h"
#include "PCB.h"
#include "ProcessLogger.h"

class Process
{
public:
	Process(int id, const std::string& n);
	~Process();

	// PCB Facade Methods
	int getPID() const;
	std::string getName() const;
	ProcessState getState() const;
	void setState(ProcessState s);
	void setStartTime(const std::string& timeStr);
	std::string getStartTime() const;
	void setCoreID(int id);
	int getCoreID() const;

	// Execution Engine Methods
	int getCommandCounter() const;
	int getTotalCommands() const;
	bool isFinished() const;

	// Executes the current command.
	void executeCurrentCommand(int coreID);

	void addCommand(std::shared_ptr<ICommand> command);
	void moveToNextLine();
	SymbolTable& getSymbolTable();

private:
	std::shared_ptr<PCB> pcb;
	std::shared_ptr<ProcessLogger> logger;

	int commandCounter; // Acts as Program Counter
	std::vector<std::shared_ptr<ICommand>> commandList;
	SymbolTable symbolTable;
};
