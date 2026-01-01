#ifndef THREAD_POOL_EXECUTOR_H
#define THREAD_POOL_EXECUTOR_H
#include "Executor.h"
#include "Job.h"
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

class ThreadPoolExecutor : public Executor {
public:
    explicit ThreadPoolExecutor(size_t n);
	ThreadPoolExecutor();
    ~ThreadPoolExecutor();

    void enqueue(std::unique_ptr<Job> work) override;

private:
    std::vector<std::thread> workers;
    std::queue<std::unique_ptr<Job>> queue;

    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
	void worker ();
};
#endif
