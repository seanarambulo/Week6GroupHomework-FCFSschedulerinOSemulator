#pragma once

#include "Process.h"
#include <queue>
#include <vector>
#include <memory>
#include <semaphore>
#include <string>

class ProcessManager {
public:
    // Singleton access
    static ProcessManager& getInstance() {
        static ProcessManager instance;
        return instance;
    }

    ProcessManager(ProcessManager const&) = delete;
    void operator=(ProcessManager const&) = delete;

    void initializeProcesses();

    // Scheduler methods
    bool isReadyQueueEmpty();
    std::shared_ptr<Process> dequeueReadyProcess();
    
    // Process tracking
    void addRunningProcess(std::shared_ptr<Process> process);
    void removeRunningProcess(std::shared_ptr<Process> process);
    void addFinishedProcess(std::shared_ptr<Process> process);

    // UI methods
    std::string getProcessList();

    // Counters or stats
    int getFinishedCount();
    int getRunningProcessesCount();

private:
    ProcessManager() = default;

    std::queue<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> runningProcesses;
    std::vector<std::shared_ptr<Process>> finishedProcesses;

    inline static const int N_THREADS_PM = 1;
    inline static const int MAX_ACCESS_PM = 1;
    inline static std::counting_semaphore<N_THREADS_PM> pmSemaphore{MAX_ACCESS_PM};
};
