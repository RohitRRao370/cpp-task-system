#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "Job.h"
#include <memory>

struct Executor {
    virtual void enqueue(std::unique_ptr<Job> work) = 0;
    virtual ~Executor() = default;
};
#endif
