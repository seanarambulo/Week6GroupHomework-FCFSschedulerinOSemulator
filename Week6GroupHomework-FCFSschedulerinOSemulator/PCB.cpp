#include "PCB.h"

PCB::PCB(int pid, const std::string& name)
	: pid(pid), name(name), state(ProcessState::READY), assignedCoreID(-1)
{
}

int PCB::getPID() const { return pid; }
std::string PCB::getName() const { return name; }

ProcessState PCB::getState() const { return state; }
void PCB::setState(ProcessState s) { state = s; }

int PCB::getCoreID() const { return assignedCoreID; }
void PCB::setCoreID(int id) { assignedCoreID = id; }

std::string PCB::getStartTime() const { return startTime; }
void PCB::setStartTime(const std::string& timeStr) { startTime = timeStr; }
