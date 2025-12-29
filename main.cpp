#include <iostream>
#include "JobSystem.h"

int printNew () {
	return 3;
}

std::string printStr () {
	return "Hello";
}

void printNothing () {
}

int main() {
	JobSystem js { 5 };

	auto f1 = js.submit(printNew);
	auto f2 = js.submit(printStr);
	auto f3 = js.submit(printNothing);

	std::cout << f1.get() << f2.get() << '\n';
	js.end();
	return 0;
}
