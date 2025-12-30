#include <iostream>
#include "benchmark.h"
#include "TaskSystem.h"
#include "tests.h"


int main() {
	// demo JobSystem initialization
	TaskSystem js { 5 };

	auto f1 = js.submit(printNew);
	auto f2 = js.submit(printStr);
	auto f3 = js.submit(printNothing);

	std::cout << f1.get() << f2.get() << '\n';
	// necessary to end threads
	js.end();

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
