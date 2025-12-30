#include <functional>
#include <iostream>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "JobSystem.h"

TaskSystem::TaskSystem(int jobs)
	: m_jobs (jobs)
{
	for (int i { 0 }; i < m_jobs; i++) {
		m_threads.emplace_back(&TaskSystem::worker, this);
	}
}

void TaskSystem::worker ()
{
	while (true) {
		Job job;
		{
			std::unique_lock<std::mutex> lock (m_queue_mutex);
			// wait for signal indicating existing task
			cv.wait (lock, [&] {
					return m_stop || !m_job_queue.empty();
					}
					);
			if (m_stop && m_job_queue.empty()) return;

			job = std::move (m_job_queue.front());
			m_job_queue.pop();
		}

		job();
	}
}
void TaskSystem::end () {
	{
		std::lock_guard<std::mutex> lock (m_queue_mutex);
		m_stop = true;
	}
	cv.notify_all();
	for (std::thread &th : m_threads) {
		th.join();
	}
}
