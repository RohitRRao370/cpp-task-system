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

		// std::cout << f3.get()
		// ^ fails since printNothing returns void
		std::cout << f1.get() << '\n' << f2.get() << '\n';
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
