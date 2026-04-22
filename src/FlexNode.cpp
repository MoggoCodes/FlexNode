#include "FlexNode.h"

#include <sstream>
#include <stdexcept>

namespace {

const char* typeName(const Type type) {
    switch (type) {
        case Type::INT: return "INT";
        case Type::DOUBLE: return "DOUBLE";
        case Type::CHAR: return "CHAR";
        case Type::STRING: return "STRING";
        case Type::BOOL: return "BOOL";
        default: return "UNINITIALIZED";
    }
}

[[noreturn]] void throwTypeMismatch(const Type expected, const Type actual) {
    throw std::runtime_error(
        "FlexNode type mismatch. Expected " + std::string(typeName(expected)) +
        ", got " + std::string(typeName(actual)) + '.'
    );
}

} // namespace

bool FlexNode::setNode(const Type newType) {
    return type == Type::UNINITIALIZED && newType != Type::UNINITIALIZED;
}

bool FlexNode::resetNode() {
    if (type == Type::UNINITIALIZED) {return false;}

    switch (type) {
        case Type::STRING:
            storageAs<std::string>()->~basic_string();
            break;
        default:
            break;
    }

    type = Type::UNINITIALIZED;
    return true;
}

FlexNode::FlexNode(const FlexNode &flexNode) : type(Type::UNINITIALIZED) {
    switch (flexNode.type) {
        case Type::INT: addInt(*flexNode.storageAs<int>()); break;
        case Type::DOUBLE: addDouble(*flexNode.storageAs<double>()); break;
        case Type::CHAR: addChar(*flexNode.storageAs<char>()); break;
        case Type::STRING: addString(*flexNode.storageAs<std::string>()); break;
        case Type::BOOL: addBool(*flexNode.storageAs<bool>()); break;
        default: break;
    }
}

FlexNode& FlexNode::operator=(const FlexNode &rhs) {
    if (this == &rhs) {
        return *this;
    }

    resetNode();
    switch (rhs.type) {
        case Type::INT: addInt(*rhs.storageAs<int>()); break;
        case Type::DOUBLE: addDouble(*rhs.storageAs<double>()); break;
        case Type::CHAR: addChar(*rhs.storageAs<char>()); break;
        case Type::STRING: addString(*rhs.storageAs<std::string>()); break;
        case Type::BOOL: addBool(*rhs.storageAs<bool>()); break;
        default: break;
    }

    return *this;
}

bool FlexNode::addInt(const int &intToAdd) {
    if (!setNode(Type::INT)) {return false;}
    new (storage) int(intToAdd);
    type = Type::INT;
    return true;
}

bool FlexNode::addDouble(const double &doubleToAdd) {
    if (!setNode(Type::DOUBLE)) {return false;}
    new (storage) double(doubleToAdd);
    type = Type::DOUBLE;
    return true;
}

bool FlexNode::addChar(const char &charToAdd) {
    if (!setNode(Type::CHAR)) {return false;}
    new (storage) char(charToAdd);
    type = Type::CHAR;
    return true;
}

bool FlexNode::addString(const std::string &strToAdd) {
    if (!setNode(Type::STRING)) {return false;}
    new (storage) std::string(strToAdd);
    type = Type::STRING;
    return true;
}

bool FlexNode::addBool(const bool &boolToAdd) {
    if (!setNode(Type::BOOL)) {return false;}
    new (storage) bool(boolToAdd);
    type = Type::BOOL;
    return true;
}

bool FlexNode::changeInt(const int &newInt) {
    if (type != Type::INT) {return false;}
    *storageAs<int>() = newInt;
    return true;
}

bool FlexNode::changeDouble(const double &newDbl) {
    if (type != Type::DOUBLE) {return false;}
    *storageAs<double>() = newDbl;
    return true;
}

bool FlexNode::changeChar(const char &newChar) {
    if (type != Type::CHAR) {return false;}
    *storageAs<char>() = newChar;
    return true;
}

bool FlexNode::changeString(const std::string &newStr) {
    if (type != Type::STRING) {return false;}
    *storageAs<std::string>() = newStr;
    return true;
}

bool FlexNode::changeBool(const bool &newBool) {
    if (type != Type::BOOL) {return false;}
    *storageAs<bool>() = newBool;
    return true;
}

int FlexNode::asInt() const {
    if (type != Type::INT) {throwTypeMismatch(Type::INT, type);}
    return *storageAs<int>();
}

double FlexNode::asDouble() const {
    if (type == Type::INT) {
        return static_cast<double>(*storageAs<int>());
    }
    if (type != Type::DOUBLE) {
        throw std::runtime_error(
            "FlexNode type mismatch. Expected DOUBLE or INT, got " + std::string(typeName(type)) + '.'
        );
    }
    return *storageAs<double>();
}

char FlexNode::asChar() const {
    if (type != Type::CHAR) {throwTypeMismatch(Type::CHAR, type);}
    return *storageAs<char>();
}

std::string FlexNode::asString() const {
    if (type == Type::CHAR) {
        return std::string(1, *storageAs<char>());
    }
    if (type != Type::STRING) {
        throw std::runtime_error(
            "FlexNode type mismatch. Expected STRING or CHAR, got " + std::string(typeName(type)) + '.'
        );
    }
    return *storageAs<std::string>();
}

bool FlexNode::asBool() const {
    if (type != Type::BOOL) {throwTypeMismatch(Type::BOOL, type);}
    return *storageAs<bool>();
}

std::ostream &FlexNode::print(std::ostream &out) const {
    switch (type) {
        case Type::INT:
            return out << this->asInt();
        case Type::DOUBLE: {
            std::ostringstream formatted;
            formatted << this->asDouble();
            std::string text = formatted.str();
            if (text.find('.') == std::string::npos &&
                text.find('e') == std::string::npos &&
                text.find('E') == std::string::npos) {
                text += ".0";
            }
            return out << text;
        }
        case Type::CHAR:
            return out << '\'' << this->asChar() << '\'';
        case Type::STRING:
            return out << '"' << this->asString() << '"';
        case Type::BOOL:
            return out << this->asBool();
        default:
            return out;
    }
}

FlexNode::~FlexNode() {
    resetNode();
}

std::ostream& operator<<(std::ostream& out, const FlexNode &flexNode) {
    return flexNode.print(out);
}
