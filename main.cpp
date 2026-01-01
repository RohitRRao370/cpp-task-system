#include <iostream>
#include "tests.h"
#include "ThreadPoolExecutor.h"
#include "TaskSystem.h"

int main () {
	ThreadPoolExecutor exec(4);
    TaskSystem tasks(exec);

    auto f1 = tasks.submit(returnMult, 2, 3);

    auto f2 = tasks.submit([] {
        throw std::runtime_error("boom");
        return 0;
    });

    std::cout << f1.get() << "\n";

    try {
        f2.get();
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }
	return 0;
}
