#ifndef JOB_SYSTEM_H
#define JOB_SYSTEM_H
#include <future>
#include <functional>
#include <queue>
#include <condition_variable>
#include <thread>
#include <mutex>

class JobSystem {
private:
	int32_t m_jobs { static_cast<int>(std::thread::hardware_concurrency()) };
	std::mutex m_queue_mutex { };
	bool m_stop { };

	using Job = std::function<void()>;
	std::queue <Job> m_job_queue;
	std::vector <std::thread> m_threads;

	std::condition_variable cv { };

public:
	void worker ();
	JobSystem (int32_t jobs);
	JobSystem () { m_jobs = static_cast<int>(std::thread::hardware_concurrency()); };

	template <typename F>
	auto submit (F&& f) -> std::future <decltype(f())>{
		using RET = decltype(f());
		auto task = std::make_shared<std::packaged_task<RET()>> (std::forward<F>(f));
		auto result = task->get_future();
		{
			std::lock_guard <std::mutex> lock (m_queue_mutex);
			m_job_queue.emplace ([task] {
					(*task)();
					}
					);
		}
		cv.notify_one();
		return result;
	}

	void end ();
};

#endif
