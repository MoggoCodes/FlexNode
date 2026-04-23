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
        recordCheck(summary, out, "setValue(INT, KeepType) succeeds when type matches",
                    node.setValue(11, SetPolicy::KeepType));
        recordCheck(summary, out, "updated int value is readable", node.asInt() == 11);
        recordCheck(summary, out, "asDouble casts INT value", node.asDouble() == 11.0);
        recordCheck(summary, out, "setValue(DOUBLE, KeepType) fails on INT node",
                    !node.setValue(3.0, SetPolicy::KeepType));
        recordCheck(summary, out, "setValue(DOUBLE, Retype) retypes node",
                    node.setValue(3.0, SetPolicy::Retype));
        recordCheck(summary, out, "node reports DOUBLE after retype", node.is(Type::DOUBLE));
        recordCheck(summary, out, "retyped double value is readable", node.asDouble() == 3.0);
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
        recordCheck(summary, out, "asString casts CHAR value", node.asString() == "1");
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
        source.setValue("beta", SetPolicy::KeepType);
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
        source.setValue(4.0, SetPolicy::KeepType);
        recordCheck(summary, out, "assignment is deep copy", assigned.asDouble() == 9.0);
    }

    out << "  Unit Summary: " << summary.passed << '/' << summary.total
        << " passed, " << summary.failed << " failed.\n";
    return summary;
}
