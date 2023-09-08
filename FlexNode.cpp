#include "FlexNode.h"

/*
todo fix string case in copy constructor
*/


bool FlexNode::setNode(Type newType) {
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
        default: return false;                
    }
    
    type = Type::UNINITIALIZED;
    containsData = false;
    return true;
}

FlexNode::FlexNode(FlexNode &flexNode) {
    switch(flexNode.type) {
        case Type::INT: addInt(*flexNode.num); break;
        case Type::DOUBLE: addDouble(*flexNode.dbl); break;
        case Type::CHAR: addChar(*flexNode.ch); break;
        //!case Type::STRING: ch = ; break;
        case Type::BOOL: addBool(*flexNode.boo); break;
        default: break;
    }
}

bool FlexNode::addInt(int intToAdd) {
    if(!setNode(Type::INT)) {return false;}
    num = new int(intToAdd);
    return true;
}

bool FlexNode::addDouble(double doubleToAdd) {
    if(!setNode(Type::DOUBLE)) {return false;}
    dbl = new double(doubleToAdd);
    return true;
}

bool FlexNode::addChar(char charToAdd) {
    if(!setNode(Type::CHAR)) {return false;}
    ch = new char(charToAdd);
    return true;
}

bool FlexNode::addString(std::string strToAdd) {
    if(!setNode(Type::STRING)) {return false;}
    ch = new char[strToAdd.length() + 1];
    copy(strToAdd.begin(), strToAdd.end(), ch);
    ch[strToAdd.length()] = '\0';
    return true;
}

bool FlexNode::addBool(bool boolToAdd) {
    if(!setNode(Type::BOOL)) {return false;}
    boo = new bool(boolToAdd);
    return true;
}

bool FlexNode::changeInt(int newInt) {
    if(type != Type::INT) {return false;}
    if(!resetNode()) {return false;}
    return addInt(newInt);
}

bool FlexNode::changeDouble(double newDbl) {
    if(type != Type::DOUBLE) {return false;}
    if(!resetNode()) {return false;}
    return addDouble(newDbl);
}

bool FlexNode::changeChar(char newChar) {
    if(type != Type::CHAR) {return false;}
    if(!resetNode()) {return false;}
    return addChar(newChar);
}

bool FlexNode::changeString(std::string newStr) {
    if(type != Type::STRING) {return false;}
    if(!resetNode()) {return false;}
    return addString(newStr);
}

bool FlexNode::changeBool(bool newBool) {
    if(type != Type::BOOL) {return false;}
    if(!resetNode()) {return false;}
    return addBool(newBool);
}

std::ostream &FlexNode::print(std::ostream &out) {
    switch(type) {
        case Type::INT: return out << *num; break;
        case Type::DOUBLE: return out << *dbl; break;
        case Type::CHAR: return out << *ch; break;
        case Type::STRING: return out << *ch; break;
        case Type::BOOL: return out << *boo; break;
        default: return out;
    }
}

FlexNode::~FlexNode() {
    resetNode();
}

std::ostream& operator<<(std::ostream& out, FlexNode flexNode) {
    return flexNode.print(out);
}