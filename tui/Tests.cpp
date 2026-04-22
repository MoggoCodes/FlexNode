#include "FlexNodeTests.h"

#include <cstdlib>
#include <iostream>

int main() {
    const TestSummary summary = runAllFlexNodeTests(std::cout);
    return summary.failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
