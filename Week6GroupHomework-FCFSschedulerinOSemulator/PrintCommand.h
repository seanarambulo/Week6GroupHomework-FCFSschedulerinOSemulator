#pragma once
#include "ICommand.h"
#include <string>

class PrintCommand : public ICommand {
public:
    PrintCommand(int pid, const std::string& toPrint);
    void execute(int coreID, const std::string& processName, ProcessLogger& logger) override;
private:
    std::string toPrint;
};
