#include "FlexNode.h"

#include <sstream>

bool FlexNode::setNode(const Type newType) {
    if(containsData) {return false;}
    containsData = true;
    type = newType;
    return true;
}

bool FlexNode::resetNode() {
    if(!containsData) {return false;}
    
    switch(type) {
        case Type::INT: delete num; num = nullptr; break;
        case Type::DOUBLE: delete dbl; dbl = nullptr; break;
        case Type::CHAR: delete ch; ch = nullptr; break;
        case Type::STRING: delete[] ch; ch = nullptr; break;
        case Type::BOOL: delete boo; boo = nullptr; break;
        default: break;
    }
    
    type = Type::UNINITIALIZED;
    containsData = false;
    return true;
}

FlexNode::FlexNode(const FlexNode &flexNode) {
    switch(flexNode.type) {
        case Type::INT: addInt(*flexNode.num); break;
        case Type::DOUBLE: addDouble(*flexNode.dbl); break;
        case Type::CHAR: addChar(*flexNode.ch); break;
        case Type::STRING: addString(flexNode.ch != nullptr ? flexNode.ch : ""); break;
        case Type::BOOL: addBool(*flexNode.boo); break;
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

std::ostream &FlexNode::print(std::ostream &out) const {
    switch(type) {
        case Type::INT: return out << *num; break;
        case Type::DOUBLE: {
            std::ostringstream formatted;
            formatted << *dbl;
            std::string text = formatted.str();
            if (text.find('.') == std::string::npos &&
                text.find('e') == std::string::npos &&
                text.find('E') == std::string::npos) {
                text += ".0";
            }
            return out << text;
        }
        case Type::CHAR: return out << '\'' << *ch << '\''; break;
        case Type::STRING: return out << '"' << (ch != nullptr ? ch : "") << '"'; break;
        case Type::BOOL: return out << *boo; break;
        default: return out;
    }
}

FlexNode::~FlexNode() {
    resetNode();
}

std::ostream& operator<<(std::ostream& out, const FlexNode &flexNode) {
    return flexNode.print(out);
}
