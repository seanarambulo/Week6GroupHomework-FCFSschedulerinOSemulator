#pragma once

#include "Process.h"
#include <thread>
#include <memory>
#include <semaphore>

class CPUWorkerThread {
public:
    CPUWorkerThread(int id);
    ~CPUWorkerThread();

    // Starts the background thread
    void start();
    
    // Assigns a process to this core.
    void assignProcess(std::shared_ptr<Process> p);

    // Checks if the core is currently idle
    bool isIdle();

    // Signal to stop the thread gracefully
    void stop();

    // Sets the semaphore used to notify the scheduler
    void setSchedulerSemaphore(std::counting_semaphore<4>* sem);

private:
    void run();
    void executeProcess(std::shared_ptr<Process> pToExecute);

    int coreID;
    std::thread workerThread;
    
    std::shared_ptr<Process> currentProcess;
    
    std::binary_semaphore stateMutex{1};
    std::binary_semaphore assignSignal{0};

    bool running;
    bool isAvailable;
    
    std::counting_semaphore<4>* schedulerSemaphore = nullptr;
};