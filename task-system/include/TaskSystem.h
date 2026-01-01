#ifndef TASK_SYSTEM_H
#define TASK_SYSTEM_H
#include "Executor.h"
#include <future>

class TaskSystem {
public:
    explicit TaskSystem(Executor& exec) : executor (exec) { }

    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
	-> std::future<std::invoke_result_t<F, Args...>> {
		using RET = std::invoke_result_t<F, Args...>;

		struct TaskJob : Job {
			std::packaged_task<RET()> task;

			explicit TaskJob(std::packaged_task<RET()>&& t)
				: task(std::move(t)) {}

			void execute() override {
				task();
			}
		};
		std::packaged_task<RET()> pt(
				[func = std::forward<F>(f),
				tup  = std::make_tuple(std::forward<Args>(args)...)
				]() mutable -> RET {
				return std::apply(func, std::move(tup));
				}
				);

		std::future<RET> result = pt.get_future();

		executor.enqueue(
				std::make_unique<TaskJob>(std::move (pt))
				);

		return result;
	}

private:
    Executor& executor;
};

#endif
