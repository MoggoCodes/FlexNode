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
    if(type != Type::UNINITIALIZED || newType == Type::UNINITIALIZED) {return false;}
    type = newType;
    return true;
}

bool FlexNode::resetNode() {
    if(type == Type::UNINITIALIZED) {return false;}
    
    switch(type) {
        case Type::INT: delete num; num = nullptr; break;
        case Type::DOUBLE: delete dbl; dbl = nullptr; break;
        case Type::CHAR: delete ch; ch = nullptr; break;
        case Type::STRING: delete[] ch; ch = nullptr; break;
        case Type::BOOL: delete boo; boo = nullptr; break;
        default: break;
    }
    
    type = Type::UNINITIALIZED;
    return true;
}

FlexNode::FlexNode(const FlexNode &flexNode) : type(Type::UNINITIALIZED) {
    switch(flexNode.type) {
        case Type::INT: addInt(flexNode.asInt()); break;
        case Type::DOUBLE: addDouble(flexNode.asDouble()); break;
        case Type::CHAR: addChar(flexNode.asChar()); break;
        case Type::STRING: addString(flexNode.asString()); break;
        case Type::BOOL: addBool(flexNode.asBool()); break;
        default: break;
    }
}

FlexNode& FlexNode::operator=(const FlexNode &rhs) {
    if(this == &rhs) {
        return *this;
    }

    resetNode();
    switch(rhs.type) {
        case Type::INT: addInt(*rhs.num); break;
        case Type::DOUBLE: addDouble(*rhs.dbl); break;
        case Type::CHAR: addChar(*rhs.ch); break;
        case Type::STRING: addString(rhs.ch != nullptr ? rhs.ch : ""); break;
        case Type::BOOL: addBool(*rhs.boo); break;
        default: break;
    }

    return *this;
}

bool FlexNode::addInt(const int &intToAdd) {
    if(!setNode(Type::INT)) {return false;}
    num = new int(intToAdd);
    return true;
}

bool FlexNode::addDouble(const double &doubleToAdd) {
    if(!setNode(Type::DOUBLE)) {return false;}
    dbl = new double(doubleToAdd);
    return true;
}

bool FlexNode::addChar(const char &charToAdd) {
    if(!setNode(Type::CHAR)) {return false;}
    ch = new char(charToAdd);
    return true;
}

bool FlexNode::addString(const std::string &strToAdd) {
    if(!setNode(Type::STRING)) {return false;}
    ch = new char[strToAdd.length() + 1];
    for(std::string::size_type i = 0; i < strToAdd.length(); ++i) {
        ch[i] = strToAdd[i];
    }
    ch[strToAdd.length()] = '\0';
    return true;
}

bool FlexNode::addBool(const bool &boolToAdd) {
    if(!setNode(Type::BOOL)) {return false;}
    boo = new bool(boolToAdd);
    return true;
}

bool FlexNode::changeInt(const int &newInt) {
    if(type != Type::INT) {return false;}
    if(!resetNode()) {return false;}
    return addInt(newInt);
}

bool FlexNode::changeDouble(const double &newDbl) {
    if(type != Type::DOUBLE) {return false;}
    if(!resetNode()) {return false;}
    return addDouble(newDbl);
}

bool FlexNode::changeChar(const char &newChar) {
    if(type != Type::CHAR) {return false;}
    if(!resetNode()) {return false;}
    return addChar(newChar);
}

bool FlexNode::changeString(const std::string &newStr) {
    if(type != Type::STRING) {return false;}
    if(!resetNode()) {return false;}
    return addString(newStr);
}

bool FlexNode::changeBool(const bool &newBool) {
    if(type != Type::BOOL) {return false;}
    if(!resetNode()) {return false;}
    return addBool(newBool);
}

int FlexNode::asInt() const {
    if (type != Type::INT) {throwTypeMismatch(Type::INT, type);}
    if (num == nullptr) {throw std::runtime_error("FlexNode internal error: INT storage is null.");}
    return *num;
}

double FlexNode::asDouble() const {
    if (type != Type::DOUBLE) {throwTypeMismatch(Type::DOUBLE, type);}
    if (dbl == nullptr) {throw std::runtime_error("FlexNode internal error: DOUBLE storage is null.");}
    return *dbl;
}

char FlexNode::asChar() const {
    if (type != Type::CHAR) {throwTypeMismatch(Type::CHAR, type);}
    if (ch == nullptr) {throw std::runtime_error("FlexNode internal error: CHAR storage is null.");}
    return *ch;
}

std::string FlexNode::asString() const {
    if (type != Type::STRING) {throwTypeMismatch(Type::STRING, type);}
    if (ch == nullptr) {throw std::runtime_error("FlexNode internal error: STRING storage is null.");}
    return {ch};
}

bool FlexNode::asBool() const {
    if (type != Type::BOOL) {throwTypeMismatch(Type::BOOL, type);}
    if (boo == nullptr) {throw std::runtime_error("FlexNode internal error: BOOL storage is null.");}
    return *boo;
}

std::ostream &FlexNode::print(std::ostream &out) const {
    switch(type) {
        case Type::INT: return out << this->asInt();
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
        case Type::CHAR: return out << '\'' << this->asChar() << '\'';
        case Type::STRING: return out << '"' << this->asString() << '"';
        case Type::BOOL: return out << this->asBool();
        default: return out;
    }
}

FlexNode::~FlexNode() {
    resetNode();
}

std::ostream& operator<<(std::ostream& out, const FlexNode &flexNode) {
    return flexNode.print(out);
}
