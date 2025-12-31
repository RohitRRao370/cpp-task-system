#ifndef TASK_SYSTEM_H
#define TASK_SYSTEM_H
#include <future>
#include <functional>
#include <queue>
#include <condition_variable>
#include <thread>
#include <mutex>

struct Job {
    std::function<void()> task;

    std::atomic<int> remaining_dependencies{0};

    std::vector<Job*> dependents;
};

class TaskSystem {
private:
	int32_t m_tasks { static_cast<int>(std::thread::hardware_concurrency()) };

	// mutex to prevent race while operating on the JobSystem class
	std::mutex m_queue_mutex { };

	// flag to indicate end of tasks
	bool m_stop { };

	using Task = std::function<void()>;
	// job queue
	std::queue <Task> m_task_queue;
	// thread tracking queue
	std::vector <std::thread> m_threads;

	std::condition_variable cv { };

public:
	~TaskSystem();
	TaskSystem (int32_t tasks);
	TaskSystem () {
		m_tasks = static_cast<int>(std::thread::hardware_concurrency());
		for (int i { 0 }; i < m_tasks; i++) {
			m_threads.emplace_back(&TaskSystem::worker, this);
		}
	}

	// worker loop
	void worker ();

	// function to add tasks to jobsystem for processing
	template <typename F, typename... Args>
	auto submit (F&& f, Args&&... args) -> std::future <std::invoke_result_t<F, Args...>> {
		using RET = std::invoke_result_t<F, Args...>;
		auto task = std::make_shared<std::packaged_task<RET()>> (
				[func = std::forward<F>(f), ...params = std::forward<Args>(args)]() mutable -> RET {
				return std::invoke(func, params...);
				}
				);
		auto result = task->get_future();
		{
			std::lock_guard <std::mutex> lock (m_queue_mutex);
			m_task_queue.emplace ([task] {
					(*task)();
					}
					);
		}
		cv.notify_one();
		return result;
	}

};

#endif
