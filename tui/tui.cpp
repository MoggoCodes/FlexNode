#include "FlexNode.h"
#include "FlexNodeTests.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string typeToString(const Type type) {
    switch (type) {
        case Type::INT: return "INT";
        case Type::DOUBLE: return "DOUBLE";
        case Type::CHAR: return "CHAR";
        case Type::STRING: return "STRING";
        case Type::BOOL: return "BOOL";
        default: return "UNINITIALIZED";
    }
}

bool readLine(const std::string &prompt, std::string &line) {
    std::cout << prompt;
    return static_cast<bool>(std::getline(std::cin, line));
}

template <typename T>
bool parseNumber(const std::string &line, T &value) {
    std::istringstream iss(line);
    iss >> value;
    if (!iss) {
        return false;
    }

    iss >> std::ws;
    return iss.eof();
}

bool readIntInput(const std::string &prompt, int &value) {
    std::string line;
    while (readLine(prompt, line)) {
        if (parseNumber(line, value)) {
            return true;
        }
        std::cout << "Invalid integer. Try again.\n";
    }
    return false;
}

bool readDoubleInput(const std::string &prompt, double &value) {
    std::string line;
    while (readLine(prompt, line)) {
        if (parseNumber(line, value)) {
            return true;
        }
        std::cout << "Invalid double. Try again.\n";
    }
    return false;
}

bool readCharInput(const std::string &prompt, char &value) {
    std::string line;
    while (readLine(prompt, line)) {
        if (line.size() == 1U) {
            value = line[0];
            return true;
        }
        std::cout << "Enter exactly one character.\n";
    }
    return false;
}

bool readBoolInput(const std::string &prompt, bool &value) {
    std::string line;
    while (readLine(prompt, line)) {
        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

        if (line == "true" || line == "t" || line == "yes" || line == "y" || line == "1") {
            value = true;
            return true;
        }
        if (line == "false" || line == "f" || line == "no" || line == "n" || line == "0") {
            value = false;
            return true;
        }
        std::cout << "Invalid bool. Use true/false, yes/no, or 1/0.\n";
    }
    return false;
}

void printNodeState(const std::string &label, const FlexNode &node) {
    std::cout << label << " -> ";
    if (node.isEmpty()) {
        std::cout << "[empty]\n";
        return;
    }

    std::cout << "[" << typeToString(node.getType()) << "] " << node << '\n';
}

bool pickType(Type &chosenType) {
    std::cout << "\nChoose Type\n";
    std::cout << "-----------\n";
    std::cout << "1. INT\n";
    std::cout << "2. DOUBLE\n";
    std::cout << "3. CHAR\n";
    std::cout << "4. STRING\n";
    std::cout << "5. BOOL\n";

    int selection = 0;
    if (!readIntInput("Type choice: ", selection)) {
        return false;
    }

    switch (selection) {
        case 1: chosenType = Type::INT; return true;
        case 2: chosenType = Type::DOUBLE; return true;
        case 3: chosenType = Type::CHAR; return true;
        case 4: chosenType = Type::STRING; return true;
        case 5: chosenType = Type::BOOL; return true;
        default:
            std::cout << "Unknown type choice.\n";
            return true;
    }
}

bool initializeNode(FlexNode &node, const std::string &label) {
    if (!node.isEmpty()) {
        std::cout << label << " is already initialized as " << typeToString(node.getType())
                  << ". Clear it before re-initializing.\n";
        return true;
    }

    Type chosenType = Type::UNINITIALIZED;
    if (!pickType(chosenType)) {
        return false;
    }
    if (chosenType == Type::UNINITIALIZED) {
        return true;
    }

    bool success = false;
    switch (chosenType) {
        case Type::INT: {
            int value = 0;
            if (!readIntInput("Enter INT value: ", value)) {
                return false;
            }
            success = node.addInt(value);
            break;
        }
        case Type::DOUBLE: {
            double value = 0.0;
            if (!readDoubleInput("Enter DOUBLE value: ", value)) {
                return false;
            }
            success = node.addDouble(value);
            break;
        }
        case Type::CHAR: {
            char value = '\0';
            if (!readCharInput("Enter CHAR value: ", value)) {
                return false;
            }
            success = node.addChar(value);
            break;
        }
        case Type::STRING: {
            std::string value;
            if (!readLine("Enter STRING value: ", value)) {
                return false;
            }
            success = node.addString(value);
            break;
        }
        case Type::BOOL: {
            bool value = false;
            if (!readBoolInput("Enter BOOL value: ", value)) {
                return false;
            }
            success = node.addBool(value);
            break;
        }
        default: break;
    }

    std::cout << (success ? "Initialization succeeded.\n" : "Initialization failed.\n");
    return true;
}

bool modifyNode(FlexNode &node, const std::string &label) {
    if (node.isEmpty()) {
        std::cout << label << " is empty. Initialize first.\n";
        return true;
    }

    bool success = false;
    switch (node.getType()) {
        case Type::INT: {
            int value = 0;
            if (!readIntInput("Enter new INT value: ", value)) {
                return false;
            }
            success = node.setValue(value, SetPolicy::KeepType);
            break;
        }
        case Type::DOUBLE: {
            double value = 0.0;
            if (!readDoubleInput("Enter new DOUBLE value: ", value)) {
                return false;
            }
            success = node.setValue(value, SetPolicy::KeepType);
            break;
        }
        case Type::CHAR: {
            char value = '\0';
            if (!readCharInput("Enter new CHAR value: ", value)) {
                return false;
            }
            success = node.setValue(value, SetPolicy::KeepType);
            break;
        }
        case Type::STRING: {
            std::string value;
            if (!readLine("Enter new STRING value: ", value)) {
                return false;
            }
            success = node.setValue(value, SetPolicy::KeepType);
            break;
        }
        case Type::BOOL: {
            bool value = false;
            if (!readBoolInput("Enter new BOOL value: ", value)) {
                return false;
            }
            success = node.setValue(value, SetPolicy::KeepType);
            break;
        }
        default:
            std::cout << "Node type is not initialized.\n";
            return true;
    }

    std::cout << (success ? "Update succeeded.\n" : "Update failed.\n");
    return true;
}

void clearNode(FlexNode &node, const std::string &label) {
    const bool success = node.clear();
    if (success) {
        std::cout << label << " cleared.\n";
    } else {
        std::cout << label << " was already empty.\n";
    }
}

void printTestResult(const TestSummary &summary) {
    if (summary.failed == 0) {
        std::cout << "Selected tests passed.\n";
    } else {
        std::cout << "Selected tests reported failures.\n";
    }
}

void printTestMenu() {
    std::cout << "\nRun Tests\n";
    std::cout << "---------\n";
    std::cout << "1. Run all tests\n";
    std::cout << "2. Comprehensive unit tests\n";
    std::cout << "3. Workflow A: Numeric Lifecycle\n";
    std::cout << "4. Workflow B: Text and Boolean Lifecycle\n";
    std::cout << "5. Workflow C: Copy and Assignment Lifecycle\n";
    std::cout << "6. Workflow D: Mismatch Handling Lifecycle\n";
    std::cout << "0. Back\n";
}

bool runTestsMenu() {
    while (true) {
        printTestMenu();

        int choice = -1;
        if (!readIntInput("Select test option: ", choice)) {
            return false;
        }

        switch (choice) {
            case 1:
                printTestResult(runAllFlexNodeTests(std::cout));
                break;
            case 2:
                printTestResult(runComprehensiveUnitTests(std::cout));
                break;
            case 3:
                printTestResult(runWorkflowA(std::cout));
                break;
            case 4:
                printTestResult(runWorkflowB(std::cout));
                break;
            case 5:
                printTestResult(runWorkflowC(std::cout));
                break;
            case 6:
                printTestResult(runWorkflowD(std::cout));
                break;
            case 0:
                return true;
            default:
                std::cout << "Unknown test menu choice.\n";
                break;
        }
    }
}

void printMenu(const FlexNode &node) {
    std::cout << "\nCurrent Node State\n";
    std::cout << "------------------\n";
    printNodeState("Node", node);

    std::cout << "\nFlexNode TUI\n";
    std::cout << "------------\n";
    if (node.isEmpty()) {
        std::cout << "1. Initialize node\n";
        std::cout << "2. Run tests\n";
    } else {
        std::cout << "1. Modify node\n";
        std::cout << "2. Clear node\n";
        std::cout << "3. Run tests\n";
    }
    std::cout << "0. Exit\n";
}

} // namespace

int main() {
    std::cout << std::boolalpha;
    std::cout << "Welcome to FlexNode interactive mode.\n";

    FlexNode node;

    while (true) {
        printMenu(node);

        int choice = -1;
        if (!readIntInput("Select option: ", choice)) {
            std::cout << "\nInput closed. Exiting.\n";
            break;
        }

        if (node.isEmpty()) {
            switch (choice) {
                case 1:
                    if (!initializeNode(node, "Node")) {
                        std::cout << "\nInput closed. Exiting.\n";
                        return EXIT_SUCCESS;
                    }
                    break;
                case 2:
                    if (!runTestsMenu()) {
                        std::cout << "\nInput closed. Exiting.\n";
                        return EXIT_SUCCESS;
                    }
                    break;
                case 0:
                    std::cout << "Exiting FlexNode TUI.\n";
                    return EXIT_SUCCESS;
                default:
                    std::cout << "Unknown menu choice.\n";
                    break;
            }
            continue;
        }

        switch (choice) {
            case 1:
                if (!modifyNode(node, "Node")) {
                    std::cout << "\nInput closed. Exiting.\n";
                    return EXIT_SUCCESS;
                }
                break;
            case 2:
                clearNode(node, "Node");
                break;
            case 3:
                if (!runTestsMenu()) {
                    std::cout << "\nInput closed. Exiting.\n";
                    return EXIT_SUCCESS;
                }
                break;
            case 0:
                std::cout << "Exiting FlexNode TUI.\n";
                return EXIT_SUCCESS;
            default:
                std::cout << "Unknown menu choice.\n";
                break;
        }
    }

    return EXIT_SUCCESS;
}
