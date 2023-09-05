#pragma once

#include <string>
#include <iostream>

enum class Type {UNINITIALIZED, INT, DOUBLE, CHAR, STRING, BOOL};

class FlexNode {
private:
    bool containsData = false;
    Type type = Type::UNINITIALIZED;

    int* num = nullptr;
    double* dbl = nullptr;
    char* ch = nullptr;
    bool* boo = nullptr;

    bool setNode(Type newType);
    bool resetNode();

public:
    FlexNode() {}
    
    bool isEmpty() {return !containsData;}
    bool clear() {return resetNode();}
    Type getType() {return type;}
    
    bool addInt(int intToAdd);
    bool addDouble(double doubleToAdd);
    bool addChar(char charToAdd);
    bool addString(std::string strToAdd);
    bool addBool(bool boolToAdd);

    bool changeInt(int newInt);
    bool changeDouble(double newDbl);
    bool changeChar(char newChar);
    bool changeString(std::string newStr);
    bool changeBool(bool newBool);

    std::ostream& print(std::ostream& out);
};