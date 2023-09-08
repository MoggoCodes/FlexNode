#pragma once

#include <string>
#include <iostream>
/*
todo Define Big 3
todo define accessors to dereference member pointers and return value
? Should this be refactored into using a union
*/

enum class Type {UNINITIALIZED, INT, DOUBLE, CHAR, STRING, BOOL};

class FlexNode {
private:
    //*MetaData
    bool containsData = false;
    Type type = Type::UNINITIALIZED;

    //* Private Data
    int* num = nullptr;
    double* dbl = nullptr;
    char* ch = nullptr;
    bool* boo = nullptr;

    bool setNode(Type newType);
    bool resetNode();

    //*Private Data Accessor

public:
    //*Constructors (Default & Copy)
    FlexNode() {type = Type::UNINITIALIZED;}
    FlexNode(FlexNode &flexNode);

    //*MetaData Accessors
    bool isEmpty() {return !containsData;}
    Type getType() {return type;}

    //*Initialize
    bool addInt(int intToAdd);
    bool addDouble(double doubleToAdd);
    bool addChar(char charToAdd);
    bool addString(std::string strToAdd);
    bool addBool(bool boolToAdd);

    //*Uninitialize
    bool clear() {return resetNode();}

    //*Modify
    bool changeInt(int newInt);
    bool changeDouble(double newDbl);
    bool changeChar(char newChar);
    bool changeString(std::string newStr);
    bool changeBool(bool newBool);

    //*Overloaded Operators
    //! FlexNode& operator=(const FlexNode &rhs);
    std::ostream& print(std::ostream& out);

    //*Destructor
    ~FlexNode();
};