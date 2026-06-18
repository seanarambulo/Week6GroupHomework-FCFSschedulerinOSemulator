#pragma once

#include "FCFSSchedulerThread.h"
#include <string>
#include <memory>

class MultiplexerConsole {
public:
    MultiplexerConsole(std::shared_ptr<FCFSSchedulerThread> scheduler);
    ~MultiplexerConsole();

    // Starts the event-driven front-end loop
    void run();

private:
    bool running = true;
    std::shared_ptr<FCFSSchedulerThread> scheduler;
    void printHelp();
    void processChar(char c, std::string& currentCommand);
    void handleInput(std::string& currentCommand);
    void processCommand(const std::string& command);
};
