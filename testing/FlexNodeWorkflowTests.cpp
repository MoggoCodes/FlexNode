#include "FlexNodeTests.h"

#include "flexnode/FlexNode.h"

#include <sstream>
#include <stdexcept>
#include <string>

using flexnode::FlexNode;
using flexnode::SetPolicy;
using flexnode::Type;

namespace {

void recordStep(TestSummary &summary, std::ostream &out, const std::string &step, const bool passed) {
    ++summary.total;
    if (passed) {
        ++summary.passed;
        out << "  [PASS] " << step << '\n';
    } else {
        ++summary.failed;
        out << "  [FAIL] " << step << '\n';
    }
}

template <typename Callable>
void recordRuntimeThrow(TestSummary &summary, std::ostream &out, const std::string &step, Callable &&callable) {
    try {
        callable();
    } catch (const std::runtime_error &) {
        recordStep(summary, out, step, true);
        return;
    } catch (...) {
    }
    recordStep(summary, out, step, false);
}

TestSummary combine(const TestSummary &a, const TestSummary &b) {
    TestSummary merged;
    merged.total = a.total + b.total;
    merged.passed = a.passed + b.passed;
    merged.failed = a.failed + b.failed;
    return merged;
}

} // namespace

TestSummary runWorkflowA(std::ostream &out) {
    out << "\n Workflow A: numeric lifecycle\n";
    TestSummary summary;

    FlexNode node;
    recordStep(summary, out, "A1 node starts empty", node.isEmpty());
    recordStep(summary, out, "A2 addInt succeeds", node.addInt(1));
    recordStep(summary, out, "A3 asInt reads 1", node.asInt() == 1);
    recordStep(summary, out, "A4 asDouble casts INT value to 1.0", node.asDouble() == 1.0);
    recordStep(summary, out, "A5 setValue(INT, KeepType) to 2 succeeds", node.setValue(2, SetPolicy::KeepType));
    recordStep(summary, out, "A6 asInt reads 2", node.asInt() == 2);
    recordStep(summary, out, "A7 asDouble casts INT value to 2.0", node.asDouble() == 2.0);
    recordStep(summary, out, "A8 clear succeeds", node.clear());
    recordStep(summary, out, "A9 addDouble succeeds after clear", node.addDouble(5));
    recordStep(summary, out, "A10 asDouble reads 5.0", node.asDouble() == 5.0);
    std::ostringstream formatted;
    formatted << node;
    recordStep(summary, out, "A11 double output remains 5.0", formatted.str() == "5.0");
    recordStep(summary, out, "A12 setValue(DOUBLE, KeepType) to 8.5 succeeds", node.setValue(8.5, SetPolicy::KeepType));
    recordStep(summary, out, "A13 asDouble reads 8.5", node.asDouble() == 8.5);

    out << "  Workflow A Summary: " << summary.passed << '/' << summary.total
        << " passed, " << summary.failed << " failed.\n";
    return summary;
}

TestSummary runWorkflowB(std::ostream &out) {
    out << "\n Workflow B: text and boolean lifecycle\n";
    TestSummary summary;

    FlexNode node;
    recordStep(summary, out, "B1 addChar succeeds", node.addChar('x'));
    recordStep(summary, out, "B2 asString casts CHAR value", node.asString() == "x");
    std::ostringstream charOut;
    charOut << node;
    recordStep(summary, out, "B3 char output is quoted", charOut.str() == "'x'");
    recordStep(summary, out, "B4 clear succeeds", node.clear());
    recordStep(summary, out, "B5 addString succeeds", node.addString("hello"));
    recordStep(summary, out, "B6 asString reads hello", node.asString() == "hello");
    recordStep(summary, out, "B7 setValue(STRING, KeepType) to world succeeds",
               node.setValue("world", SetPolicy::KeepType));
    recordStep(summary, out, "B8 asString reads world", node.asString() == "world");
    std::ostringstream stringOut;
    stringOut << node;
    recordStep(summary, out, "B9 string output is quoted", stringOut.str() == "\"world\"");
    recordStep(summary, out, "B10 clear succeeds again", node.clear());
    recordStep(summary, out, "B11 addBool succeeds", node.addBool(true));
    recordStep(summary, out, "B12 asBool reads true", node.asBool());
    recordStep(summary, out, "B13 setValue(BOOL, KeepType) to false succeeds",
               node.setValue(false, SetPolicy::KeepType));
    recordStep(summary, out, "B14 asBool reads false", !node.asBool());

    out << "  Workflow B Summary: " << summary.passed << '/' << summary.total
        << " passed, " << summary.failed << " failed.\n";
    return summary;
}

TestSummary runWorkflowC(std::ostream &out) {
    out << "\n Workflow C: copy and assignment across steps\n";
    TestSummary summary;

    FlexNode source;
    recordStep(summary, out, "C1 source addString(root) succeeds", source.addString("root"));

    FlexNode copied(source);
    recordStep(summary, out, "C2 copy constructor preserved root", copied.asString() == "root");

    recordStep(summary, out, "C3 source setValue(STRING, KeepType) to updated succeeds",
               source.setValue("updated", SetPolicy::KeepType));
    recordStep(summary, out, "C4 source now reads updated", source.asString() == "updated");
    recordStep(summary, out, "C5 copied still reads root", copied.asString() == "root");

    FlexNode assigned;
    assigned.addInt(99);
    assigned = source;
    recordStep(summary, out, "C6 assignment changed target type to STRING", assigned.is(Type::STRING));
    recordStep(summary, out, "C7 assignment copied updated value", assigned.asString() == "updated");

    recordStep(summary, out, "C8 source clear succeeds", source.clear());
    recordStep(summary, out, "C9 source is empty after clear", source.isEmpty());
    recordStep(summary, out, "C10 assigned remains updated", assigned.asString() == "updated");

    out << "  Workflow C Summary: " << summary.passed << '/' << summary.total
        << " passed, " << summary.failed << " failed.\n";
    return summary;
}

TestSummary runWorkflowD(std::ostream &out) {
    out << "\n Workflow D: explicit mismatch checks during a real flow\n";
    TestSummary summary;

    FlexNode node;
    recordStep(summary, out, "D1 addDouble succeeds", node.addDouble(3.25));
    recordStep(summary, out, "D2 asDouble succeeds", node.asDouble() == 3.25);
    recordRuntimeThrow(summary, out, "D3 asInt throws during DOUBLE flow", [&node]() { (void)node.asInt(); });
    recordStep(summary, out, "D4 clear succeeds", node.clear());
    recordStep(summary, out, "D5 addInt succeeds after clear", node.addInt(42));
    recordRuntimeThrow(summary, out, "D6 asString throws during INT flow", [&node]() { (void)node.asString(); });

    out << "  Workflow D Summary: " << summary.passed << '/' << summary.total
        << " passed, " << summary.failed << " failed.\n";
    return summary;
}

TestSummary runWorkflowTests(std::ostream &out) {
    out << "\n[Workflow Tests]\n";

    const TestSummary workflowA = runWorkflowA(out);
    const TestSummary workflowB = runWorkflowB(out);
    const TestSummary workflowC = runWorkflowC(out);
    const TestSummary workflowD = runWorkflowD(out);

    const TestSummary total = combine(
        combine(workflowA, workflowB),
        combine(workflowC, workflowD)
    );

    out << "  Workflow Summary: " << total.passed << '/' << total.total
        << " passed, " << total.failed << " failed.\n";
    return total;
}
