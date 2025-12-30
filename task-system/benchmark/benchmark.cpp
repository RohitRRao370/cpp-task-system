#include "TaskSystem.h"
#include <iostream>
#include <chrono>
#include "benchmark.h"

void benchmark() {

	auto start = std::chrono::steady_clock::now();
	std::vector<std::thread> threads;
	threads.reserve(N);

	for (int i = 0; i < N; ++i) {
		threads.emplace_back([] {
				volatile int x = 0;
				});
	}

	for (auto& t : threads)
		t.join();

	auto end = std::chrono::steady_clock::now();
	std::cout << "Standard thread benchmark: " << std::chrono::duration<double>(end - start).count() << '\n';

	auto startjs = std::chrono::steady_clock::now();
	TaskSystem js;
	std::vector<std::future<void>> futures;
	futures.reserve(N);

	for (int i = 0; i < N; ++i) {
		futures.push_back(js.submit([] {
					volatile int x = 0;
					}));
	}

	for (auto& f : futures)
		f.wait();
	js.end();
	auto endjs = std::chrono::steady_clock::now();

	std::cout << "TaskSystem benchmark: "<< std::chrono::duration<double>(endjs - startjs).count() << '\n';
}
