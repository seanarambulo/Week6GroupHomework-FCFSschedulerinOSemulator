#include "PrintCommand.h"
#include <chrono>
#include <iomanip>
#include <sstream>

PrintCommand::PrintCommand(int pid, const std::string& toPrint) : ICommand(pid, PRINT), toPrint(toPrint) {
}

void PrintCommand::execute(int coreID, const std::string& processName, ProcessLogger& logger) {
    logger.log(coreID, toPrint);
}
