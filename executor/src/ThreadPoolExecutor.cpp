#include "ThreadPoolExecutor.h"
#include <future>
#include <iostream>

ThreadPoolExecutor::ThreadPoolExecutor() {
    for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back(&ThreadPoolExecutor::worker, this);
    }
}

ThreadPoolExecutor::ThreadPoolExecutor(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        workers.emplace_back(&ThreadPoolExecutor::worker, this);
    }
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
    {
        std::lock_guard lock(mtx);
        stop = true;
    }
    cv.notify_all();

    for (auto& t : workers)
        t.join();
}

void ThreadPoolExecutor::enqueue(std::unique_ptr<Job> work) {
    {
        std::lock_guard lock(mtx);
        queue.push(std::move(work));
    }
    cv.notify_one();
}

void ThreadPoolExecutor::worker() {
	while (true) {
		std::unique_ptr<Job> work;

		{
			std::unique_lock lock(mtx);
			cv.wait(lock, [&]
					{
					return stop || !queue.empty();
					}
				   );

			if (stop && queue.empty())
				return;

			work = std::move(queue.front());
			queue.pop();
		}

		try {
			work->execute();
		} catch (...) {
			std::cout << "Error while performing task\n";
		}
	}
}

