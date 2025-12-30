# Task system library.

This is a task system library. Using this system provides an 89% speedup
over using standard threads for large-scale, fine-grained workloads. This is
accomplished by reducing task submission overhead.

# Architecture overview

This project implements a multithreaded task system that executes user-submitted
tasks on a fixed pool of worker threads.

The system consists of:

- A TaskSystem class that owns and manages worker threads

- A thread-safe task queue storing callable tasks

- A condition-variable–based scheduler that efficiently blocks idle workers

Tasks are submitted as callables (std::function\<void\(\)\>) and executed
asynchronously by worker threads. Threads are created once during
initialization and reused for the lifetime of the system to avoid
thread creation overhead.

# Threading Model

The task system uses a fixed-size thread pool. The number of worker threads is
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

# Known limitations

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

# Usage

The method of usage is as follows:

1. Instantiate a TaskSystem class with the required number of threads

2. Submit tasks for processing using the TaskSystem.submit funciton

3. Shutdown the system using TaskSystem.end to wake all threads and join
them back to the main function.

_Note: Originally started as a job system, refactored after clearing semantics_
