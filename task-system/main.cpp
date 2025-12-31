#include <iostream>
#include "benchmark.h"
#include "TaskSystem.h"
#include "tests.h"


int main() {
	// demo JobSystem initialization
	{
		TaskSystem js { 5 };

		auto f1 = js.submit(returnMult, 5, 7);
		auto f2 = js.submit(returnStr, "Hello", " world!");
		auto f3 = js.submit(printNothing);
		auto f4 = js.submit(throwException);
		auto f5 = js.submit(throwException);
		auto f6 = js.submit(throwException);
		auto f7 = js.submit(throwException);

		f3.get();
		// std::cout << f3.get()
		// ^ fails since printNothing returns void
		std::cout << f1.get() << '\n' << f2.get() << '\n';
		try {
			f4.get();
		} catch (int x) {
			std::cout << "f4 threw exception number: " << x << '\n';
		}
		try {
			f5.get();
		} catch (int x) {
			std::cout << "f5 threw exception number: " << x << '\n';
		}
		try {
			f6.get();
		} catch (int x) {
			std::cout << "f6 threw exception number: " << x << '\n';
		}
		try {
			f7.get();
		} catch (int x) {
			std::cout << "f7 threw exception number: " << x << '\n';
		}
	}

	std::cout << "Execute benchmarks?(Y or n)\n";
	char option;
	std::cin >> option;
	if (option == 'n')
		;
	else {
		benchmark();
	}
	return 0;
}
