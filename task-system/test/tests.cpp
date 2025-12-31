#include "tests.h"

// return int
int returnMult (int a, int b) {
	return a * b;
}

// return string
std::string returnStr (std::string start, std::string end) {
	return start + end;
}
void throwException () {
	throw -1;
}

// display functionality with void return values
void printNothing() {
}
