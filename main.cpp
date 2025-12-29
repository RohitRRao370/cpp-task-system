#include <iostream>
#include "benchmark.h"
#include "JobSystem.h"
#include "tests.h"

void printNothing () {
}

int main() {
	JobSystem js { 5 };

	auto f1 = js.submit(printNew);
	auto f2 = js.submit(printStr);
	auto f3 = js.submit(printNothing);

	std::cout << f1.get() << f2.get() << '\n';
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
