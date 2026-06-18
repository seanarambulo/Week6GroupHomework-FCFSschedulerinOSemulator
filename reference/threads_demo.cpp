#include "ThreadWorker.h"
#include <iostream>
#include <vector>

// Compilation:
// g++ -std=c++20 threads_demo.cpp ThreadWorker.cpp -o threads_demo


int main() {
    std::vector<ThreadWorker> workers;
    std::vector<std::thread> threads;

    int n_turns = 10;

    for (int i = 1; i <= ThreadWorker::N_THREADS; i++) {
        workers.push_back(ThreadWorker(i, n_turns));
    }

    // Multiple threads independently counting 1 to n_turns 
    std::cout << "--- Running asynchronous threads ---" << std::endl;
    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads.push_back(std::thread(&ThreadWorker::run_async, &workers[i]));
    }

    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads[i].join();
    }
    threads.clear();
    std::cout << "--- Finished asynchronous threads ---" << std::endl;
    std::cout << std::endl;

    // Multiple threads taking turns counting 1 to n_turns while maintaining order 
    std::cout << "--- Running synchronous threads ---" << std::endl;
    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads.push_back(std::thread(&ThreadWorker::run_sync, &workers[i]));
    }

    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads[i].join();
    }
    threads.clear();
    std::cout << "--- Finished synchronous threads ---" << std::endl;
    std::cout << std::endl;

    // Multiple threads incrementing a shared counter (order not guaranteed)
    std::cout << "--- Running threads with shared counter ---" << std::endl;
    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads.push_back(std::thread(&ThreadWorker::run_shared, &workers[i]));
    }

    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads[i].join();
    }
    threads.clear();
    std::cout << "--- Finished threads with shared counter ---" << std::endl;
    std::cout << std::endl;
}