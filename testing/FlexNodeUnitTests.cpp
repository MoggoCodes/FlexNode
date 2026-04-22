#include "FlexNodeTests.h"

#include "FlexNode.h"

#include <sstream>
#include <stdexcept>
#include <string>

namespace {

void recordCheck(TestSummary &summary, std::ostream &out, const std::string &name, const bool passed) {
    ++summary.total;
    if (passed) {
        ++summary.passed;
        out << "  [PASS] " << name << '\n';
    } else {
        ++summary.failed;
        out << "  [FAIL] " << name << '\n';
    }
}

template <typename Callable>
void recordRuntimeThrow(TestSummary &summary, std::ostream &out, const std::string &name, Callable &&callable) {
    try {
        callable();
    } catch (const std::runtime_error &) {
        recordCheck(summary, out, name, true);
        return;
    } catch (...) {
    }
    recordCheck(summary, out, name, false);
}

} // namespace

TestSummary runComprehensiveUnitTests(std::ostream &out) {
    out << "\n[Comprehensive Unit Tests]\n";
    TestSummary summary;

    {
        FlexNode node;
        recordCheck(summary, out, "default node starts empty", node.isEmpty());
        recordRuntimeThrow(summary, out, "asInt throws on empty node", [&node]() { (void)node.asInt(); });
    }

    {
        FlexNode node;
        recordCheck(summary, out, "addInt succeeds on empty node", node.addInt(7));
        recordCheck(summary, out, "node reports INT after addInt", node.is(Type::INT));
        recordCheck(summary, out, "asInt returns stored int", node.asInt() == 7);
        recordCheck(summary, out, "addDouble fails on initialized node", !node.addDouble(2.5));
        recordCheck(summary, out, "changeInt succeeds when type matches", node.changeInt(11));
        recordCheck(summary, out, "changed int value is readable", node.asInt() == 11);
        recordCheck(summary, out, "changeDouble fails when type mismatches", !node.changeDouble(3.0));
        recordRuntimeThrow(summary, out, "asDouble throws when current type is INT", [&node]() { (void)node.asDouble(); });
        recordCheck(summary, out, "clear succeeds on initialized node", node.clear());
        recordCheck(summary, out, "type resets to UNINITIALIZED after clear", node.getType() == Type::UNINITIALIZED);
        recordCheck(summary, out, "clear fails on already-empty node", !node.clear());
    }

    {
        FlexNode node;
        recordCheck(summary, out, "addDouble succeeds", node.addDouble(1.0));
        recordCheck(summary, out, "asDouble returns stored double", node.asDouble() == 1.0);
        std::ostringstream formatted;
        formatted << node;
        recordCheck(summary, out, "double output prints at least one decimal place", formatted.str() == "1.0");
    }

    {
        FlexNode node;
        recordCheck(summary, out, "addChar succeeds", node.addChar('1'));
        recordCheck(summary, out, "asChar returns stored char", node.asChar() == '1');
        std::ostringstream formatted;
        formatted << node;
        recordCheck(summary, out, "char output is single-quoted", formatted.str() == "'1'");
    }

    {
        FlexNode node;
        recordCheck(summary, out, "addString succeeds", node.addString("1"));
        recordCheck(summary, out, "asString returns stored text", node.asString() == "1");
        std::ostringstream formatted;
        formatted << node;
        recordCheck(summary, out, "string output is double-quoted", formatted.str() == "\"1\"");
    }

    {
        FlexNode node;
        recordCheck(summary, out, "addBool succeeds", node.addBool(true));
        recordCheck(summary, out, "asBool returns stored bool", node.asBool());
        recordRuntimeThrow(summary, out, "asString throws when current type is BOOL", [&node]() { (void)node.asString(); });
    }

    {
        FlexNode source;
        source.addString("alpha");
        FlexNode copied(source);
        recordCheck(summary, out, "copy constructor copies current value", copied.asString() == "alpha");
        source.changeString("beta");
        recordCheck(summary, out, "copy is independent of source changes", copied.asString() == "alpha");
    }

    {
        FlexNode source;
        source.addDouble(9.0);
        FlexNode assigned;
        assigned.addInt(3);
        assigned = source;
        recordCheck(summary, out, "assignment copies type", assigned.is(Type::DOUBLE));
        recordCheck(summary, out, "assignment copies value", assigned.asDouble() == 9.0);
        source.changeDouble(4.0);
        recordCheck(summary, out, "assignment is deep copy", assigned.asDouble() == 9.0);
    }

    out << "  Unit Summary: " << summary.passed << '/' << summary.total
        << " passed, " << summary.failed << " failed.\n";
    return summary;
}
