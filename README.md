# Task system library

## Overview

This is a task system library. Using this system provides upto an 89% speedup
over using standard threads for large-scale, fine-grained workloads. This is
accomplished by reducing task submission overhead.

## Build and run

### Requirements
- C++17 or later
- CMake ≥ 3.16
- A POSIX system (Linux/macOS tested)

### Build
```bash
git clone https://github.com/RohitRRao370/cpp-task-system.git
cd cpp-task-system
mkdir build && cd build
cmake ..
make
```

# Architecture overview

This project implements a multithreaded task system that executes user-submitted
tasks on a fixed pool of worker threads.

The system consists of:

- A ThreadPoolExecutor class that creates a base pool of threads to execute
jobs or tasks upon.

- A TaskSystem class that submits tasks to a ThreadPoolExecutor instance

- A thread-safe task queue storing callable tasks in the ThreadPoolExecutor

- A condition-variable–based scheduler that efficiently blocks idle workers

Tasks are submitted as callables (std::function\<void\(\)\>) and executed
asynchronously by worker threads. Threads are created once during
initialization and reused for the lifetime of the system to avoid
thread creation overhead.

## Executor

The ThreadPoolExecutor class creates a pool of threads each of which run in a
loop until a task is available for them in the task queue. Ownership for a job
is unique for each job.

## Task system

The task system is a light-weight wrapper on top of the executor to submit tasks
to the thread pool queue. Results of the tasks are obtained using futures.
Exceptions are propogated through said futures.

## Threading Model

The executor class uses a fixed-size thread pool. The number of worker threads is
specified by the user at construction time, defaulting to
std::thread::hardware\_concurrency().

Each worker thread runs a private loop that:

1. Acquires a mutex protecting the shared task queue

2. Waits on a condition variable until either new tasks are available or shutdown
is requested

3. Removes one task from the queue

4. Executes the task outside the critical section

Synchronization is achieved using:

- std::mutex to protect shared state

- std::condition\_variable to block idle threads without busy-waiting

Shutdown is cooperative: the destructor sets a stop flag, notifies all worker
threads, and joins them to ensure a clean termination.

## Known limitations

This implementation intentionally prioritizes correctness and clarity over
advanced scheduling features. Known limitations include:

- Tasks are executed in FIFO order; no prioritization is implemented

- The task queue uses a single mutex, which may become a contention point under
heavy load

- Tasks are stored as std::function<void()>, which introduces minor type-erasure
overhead

- There is no support for task dependencies, cancellation, or work stealing

These tradeoffs were chosen to keep the implementation simple and correct.
The design can be extended to support more advanced scheduling strategies
if required.

## Architectural evolution

The thread pool and task system started off coupled. Later the thread pool
was refactored into a separate class. This separation also permitted
move-only jobs.

## Future work

The implementation could later be updated to include a job system and
dependencies.

_Note: Originally started as a job system, refactored after clearing semantics_
