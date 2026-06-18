#include "Process.h"

Process::Process(int id, const std::string& n) : commandCounter(0) {
    pcb = std::make_shared<PCB>(id, n);
    logger = std::make_shared<ProcessLogger>(n);
}

Process::~Process() {
}

int Process::getPID() const {
    return pcb->getPID();
}

std::string Process::getName() const {
    return pcb->getName();
}

ProcessState Process::getState() const {
    return pcb->getState();
}

void Process::setState(ProcessState s) {
    pcb->setState(s);
}

void Process::setStartTime(const std::string& timeStr) {
    pcb->setStartTime(timeStr);
}

std::string Process::getStartTime() const {
    return pcb->getStartTime();
}

void Process::setCoreID(int id) {
    pcb->setCoreID(id);
}

int Process::getCoreID() const {
    return pcb->getCoreID();
}

int Process::getCommandCounter() const {
    return commandCounter;
}

int Process::getTotalCommands() const {
    return commandList.size();
}

bool Process::isFinished() const {
    return commandCounter >= commandList.size();
}

void Process::executeCurrentCommand(int coreID) {
    if (commandCounter < commandList.size()) {
        auto command = commandList[commandCounter];
        // Pass the logger to the command instead of an ofstream
        command->execute(coreID, pcb->getName(), *logger);
    }
}

void Process::addCommand(std::shared_ptr<ICommand> command) {
    commandList.push_back(command);
}

void Process::moveToNextLine() {
    commandCounter++;
}

SymbolTable& Process::getSymbolTable() {
    return symbolTable;
}
