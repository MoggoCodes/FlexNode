#include "FlexNodeTests.h"

#include <ostream>

namespace {

TestSummary combine(const TestSummary &a, const TestSummary &b) {
    TestSummary merged;
    merged.total = a.total + b.total;
    merged.passed = a.passed + b.passed;
    merged.failed = a.failed + b.failed;
    return merged;
}

} // namespace

TestSummary runAllFlexNodeTests(std::ostream &out) {
    out << "\n=== FlexNode Test Run ===\n";

    const TestSummary unit = runComprehensiveUnitTests(out);
    const TestSummary workflows = runWorkflowTests(out);
    const TestSummary total = combine(unit, workflows);

    out << "\n[Test Totals]\n";
    out << "  Passed: " << total.passed << '\n';
    out << "  Failed: " << total.failed << '\n';
    out << "  Total: " << total.total << '\n';
    if (total.failed == 0) {
        out << "  Result: PASS\n";
    } else {
        out << "  Result: FAIL\n";
    }

    return total;
}
