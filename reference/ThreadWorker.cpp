#include "ThreadWorker.h"
#include <chrono>
#include <iostream>


ThreadWorker::ThreadWorker(int i, int n) : id(i), n_turns(n) {}

// Counts 1 to n_turns
void ThreadWorker::run_async() {
    for (int i = 1; i <= this->n_turns; i++) {
        std::cout << "Thread " << this->id << ": " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(ThreadWorker::DELAY));
    }        
}

// Counts 1 to n_turns, but waits for its turn before incrementing
void ThreadWorker::run_sync() {
    while(true) {
        // Attempt to lock mutex:
        // - If successful, proceed with the next steps 
        // - If currently locked by another thread, wait until lock is released
        std::unique_lock<std::mutex> lock(ThreadWorker::turn_mutex);

        // Check if its turn:
        // - If yes, proceed with the next steps 
        // - If no, release lock to allow other threads to proceed while waiting
        ThreadWorker::turn_cv.wait(lock, [this]{ return ThreadWorker::turn == this->id; });

        // If counting is completed, update turn and notify other threads before exiting
        if (this->counter > this->n_turns) {
            ThreadWorker::turn = (ThreadWorker::turn % ThreadWorker::N_THREADS) + 1;
            ThreadWorker::turn_cv.notify_all();
            return;
        }

        std::cout << "Thread " << this->id << ": " << this->counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(ThreadWorker::DELAY));

        // Update count and pass the turn to the next thread
        this->counter++; 
        ThreadWorker::turn = (ThreadWorker::turn % ThreadWorker::N_THREADS) + 1;

        // Notify other threads so they can attempt to acquire mutex lock
        ThreadWorker::turn_cv.notify_all();

        // Mutex lock is released at the end its scope
    }
}

// Increments a shared counter n_turn times
// Only MAX_ACCESS threads can access the shared counter at a time
void ThreadWorker::run_shared() {
    for (int i = 1; i <= this->n_turns; i++) {
        // Check if access is permitted by the semaphore
        ThreadWorker::semaphore.acquire();

        std::cout << "Thread " << this->id << ": " << ThreadWorker::shared_counter 
                  << " (" << i << "/" << this->n_turns << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(ThreadWorker::DELAY));

        // Using std::atomic<int> for shared_counter ensures increment is thread-safe
        ThreadWorker::shared_counter++;

        // Release semaphore access to allow other threads to gain permission
        ThreadWorker::semaphore.release();

        // Not necessary, but may help other threads to take over sooner
        std::this_thread::yield();
    }
}