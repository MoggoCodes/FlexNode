#pragma once

#include <iosfwd>

struct TestSummary {
    int total = 0;
    int passed = 0;
    int failed = 0;
};

TestSummary runComprehensiveUnitTests(std::ostream &out);
TestSummary runWorkflowA(std::ostream &out);
TestSummary runWorkflowB(std::ostream &out);
TestSummary runWorkflowC(std::ostream &out);
TestSummary runWorkflowD(std::ostream &out);
TestSummary runWorkflowTests(std::ostream &out);
TestSummary runAllFlexNodeTests(std::ostream &out);
