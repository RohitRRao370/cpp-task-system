#include <functional>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "TaskSystem.h"

TaskSystem::TaskSystem(int32_t tasks)
	: m_tasks (tasks)
{
	for (int i { 0 }; i < m_tasks; i++) {
		m_threads.emplace_back(&TaskSystem::worker, this);
	}
}

void TaskSystem::worker ()
{
	while (true) {
		Task task;
		{
			std::unique_lock<std::mutex> lock (m_queue_mutex);
			// wait for signal indicating existing task
			cv.wait (lock, [&] {
					return m_stop || !m_task_queue.empty();
					}
					);
			if (m_stop && m_task_queue.empty()) return;

			task = std::move (m_task_queue.front());
			m_task_queue.pop();
		}

		task();
	}
}
TaskSystem::~TaskSystem () {
	{
		std::lock_guard<std::mutex> lock (m_queue_mutex);
		m_stop = true;
	}
	cv.notify_all();
	for (std::thread &th : m_threads) {
		th.join();
	}
}
