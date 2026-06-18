# OS Emulator Architecture

Here is the updated architecture diagram representing your current codebase, including the newly added Command pattern (`ICommand`, `PrintCommand`) and the multi-threaded scheduling system.

## Class Architecture Diagram

This diagram shows the relationships, ownership, and synchronization primitives between the core classes.

```mermaid
classDiagram
    class Week6GroupHomework {
        <<Main Entry Point>>
    }

    class ProcessManager {
        <<Singleton>>
        -queue~Process~ readyQueue
        -vector~Process~ runningProcesses
        -vector~Process~ finishedProcesses
        -binary_semaphore pmSemaphore
        +getInstance()
        +dequeueReadyProcess()
        +addRunningProcess()
        +addFinishedProcess()
    }

    class FCFSSchedulerThread {
        -int numberOfCores
        -vector~CPUWorkerThread~ cpuCores
        -counting_semaphore~4~ coreSemaphore
        +start()
        +run()
    }

    class CPUWorkerThread {
        -int coreID
        -Process currentProcess
        -binary_semaphore stateMutex
        -binary_semaphore assignSignal
        +start()
        +assignProcess()
        +executeProcess()
        -run()
    }

    class Process {
        -int pid
        -string name
        -ProcessState state
        -int commandCounter
        -vector~ICommand~ commandList
        -SymbolTable symbolTable
        -ofstream logFile
        +executeCurrentCommand()
        +moveToNextLine()
    }

    class ICommand {
        <<Interface>>
        +execute(coreID, processName, logFile)*
    }

    class PrintCommand {
        -string toPrint
        +execute(coreID, processName, logFile)
    }

    class SymbolTable {
        -unordered_map~string, string~ variables
    }

    Week6GroupHomework --> ProcessManager : Initializes
    Week6GroupHomework --> FCFSSchedulerThread : Starts

    FCFSSchedulerThread --> ProcessManager : Polls readyQueue
    FCFSSchedulerThread *-- CPUWorkerThread : Owns multiple cores

    CPUWorkerThread --> Process : Executes assigned process
    Process *-- ICommand : Owns list of commands
    Process *-- SymbolTable : Owns variable table
    
    ICommand <|.. PrintCommand : Implements
```

## System Workflow (Sequence Diagram)

This sequence diagram illustrates the lifecycle of a Process as it moves from the `ProcessManager`, gets picked up by the `FCFSSchedulerThread`, and is executed by a `CPUWorkerThread`.

```mermaid
sequenceDiagram
    participant PM as ProcessManager
    participant Sched as FCFSSchedulerThread
    participant Core as CPUWorkerThread
    participant Proc as Process
    participant Cmd as ICommand (PrintCommand)

    note over PM: Initializes 10 Processes<br/>Adds to readyQueue
    
    loop While Emulator is Running
        Sched->>PM: isReadyQueueEmpty()
        alt Queue Not Empty
            Sched->>Sched: coreSemaphore.try_acquire_for() (Wait for available core)
            Sched->>Core: isIdle()
            Sched->>PM: dequeueReadyProcess()
            PM-->>Sched: returns std::shared_ptr<Process>
            
            Sched->>Core: assignProcess(Process)
            note right of Core: Core wakes up via assignSignal
            
            Core->>Proc: executeProcess(Process)
            
            loop Until Process isFinished()
                Proc->>Cmd: execute(coreID, processName, logFile)
                Cmd-->>Proc: writes to logFile
                Proc->>Proc: moveToNextLine() (commandCounter++)
            end
            
            Core->>PM: removeRunningProcess(Process)
            Core->>PM: addFinishedProcess(Process)
            Core->>Sched: coreSemaphore.release() (Signals core is free)
            note right of Core: Core goes back to sleep
        end
    end
```
