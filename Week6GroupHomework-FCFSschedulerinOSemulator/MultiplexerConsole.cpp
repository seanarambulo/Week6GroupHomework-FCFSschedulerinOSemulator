#include "MultiplexerConsole.h"
#include "ProcessManager.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>

MultiplexerConsole::MultiplexerConsole(std::shared_ptr<FCFSSchedulerThread> sched) : scheduler(sched) {
}

MultiplexerConsole::~MultiplexerConsole() {
}

void MultiplexerConsole::printHelp() {
    std::cout << "Available commands:\n";
    std::cout << "  screen -ls   : Show process status\n";
    std::cout << "  report-util  : Generate a report of the screen -ls command\n";
    std::cout << "  print        : Display process logs\n";
    std::cout << "  exit         : Terminate the emulator\n";
}

void MultiplexerConsole::processCommand(const std::string& command) {
    if (command == "exit") {
        running = false;
    } else if (command == "screen -ls") {
        int totalCores = scheduler ? scheduler->getNumCores() : 4;
        int coresUsed = ProcessManager::getInstance().getRunningProcessesCount();
        int coresAvailable = totalCores - coresUsed;
        int cpuUtil = totalCores > 0 ? (coresUsed * 100 / totalCores) : 0;
        
        std::cout << "CPU utilization: " << cpuUtil << "%\n";
        std::cout << "Cores used: " << coresUsed << "\n";
        std::cout << "Cores available: " << coresAvailable << "\n\n";
        std::cout << ProcessManager::getInstance().getProcessList();
    } else if (command == "report-util") {
        std::ofstream file("c:/csopesy-log.txt");
        if (file.is_open()) {
            int totalCores = scheduler ? scheduler->getNumCores() : 4;
            int coresUsed = ProcessManager::getInstance().getRunningProcessesCount();
            int coresAvailable = totalCores - coresUsed;
            int cpuUtil = totalCores > 0 ? (coresUsed * 100 / totalCores) : 0;

            file << "CPU utilization: " << cpuUtil << "%\n";
            file << "Cores used: " << coresUsed << "\n";
            file << "Cores available: " << coresAvailable << "\n\n";
            file << ProcessManager::getInstance().getProcessList();
            file.close();
            std::cout << "Report generated at C:/csopesy-log.txt!\n";
        } else {
            std::cout << "Failed to generate report at C:/csopesy-log.txt!\n";
        }
    } else if (command == "print") {
        std::string processName;
        std::cout << "Enter process name: ";
        std::getline(std::cin, processName);
        std::cout << "\n";
        
        std::string filename = processName + ".txt";
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << "\n";
            }
            file.close();
        } else {
            std::cout << "Process " << processName << " not found.\n";
        }
    } else if (!command.empty()) {
        std::cout << "Unknown command: " << command << "\n";
        printHelp();
    }
}

void MultiplexerConsole::processChar(char c, std::string& currentCommand) {
    if (c == '\r' || c == '\n') {
        std::cout << "\n";
        processCommand(currentCommand);
        currentCommand.clear();
        if (running) {
            std::cout << "root:\\> ";
        }
        return;
    } 
    
    if (c == '\b') {
        if (!currentCommand.empty()) {
            currentCommand.pop_back();
            std::cout << "\b \b";
        }
        return;
    }
    
    currentCommand += c;
    std::cout << c;
}

void MultiplexerConsole::handleInput(std::string& currentCommand) {
    while (_kbhit()) {
        processChar(_getch(), currentCommand);
    }
}

void MultiplexerConsole::run() {
    std::cout << "  / ____|  / ____|/ __ \\|  __ \\|  ____|/ ____|\\ \\   / /\n";
    std::cout << " | |      | (___ | |  | | |__) | |__  | (___   \\ \\_/ / \n";
    std::cout << " | |       \\___ \\| |  | |  ___/|  __|  \\___ \\   \\   /  \n";
    std::cout << " | |____   ____) | |__| | |    | |____ ____) |   | |   \n";
    std::cout << "  \\_____| |_____/ \\____/|_|    |______|_____/    |_|   \n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << "Welcome to CSOPESY Emulator!\n\n";
    std::cout << "Developers:\n";
    std::cout << "Arambulo, Sean Marthy D.\n\n";
    std::cout << "Last updated: 06-17-2026\n\n";
    std::cout << "root:\\> ";
    std::string currentCommand = "";

    while (running) {
        handleInput(currentCommand);  
    }

    std::cout << "Exiting OS Emulator...\n";
    if (scheduler) {
        scheduler->stop();
    }
}
