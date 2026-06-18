#pragma once
#include <string>

enum class ProcessState {
    READY,
    RUNNING,
    WAITING,
    FINISHED
};

class PCB {
public:
    PCB(int pid, const std::string& name);

    int getPID() const;
    std::string getName() const;
    
    ProcessState getState() const;
    void setState(ProcessState state);
    
    int getCoreID() const;
    void setCoreID(int coreID);
    
    std::string getStartTime() const;
    void setStartTime(const std::string& startTime);

private:
    int pid;
    std::string name;
    ProcessState state;
    int assignedCoreID;
    std::string startTime;
};
