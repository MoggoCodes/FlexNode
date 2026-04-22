#pragma once

#include <iostream>
#include <string>
/*
todo Define Big 3
todo define accessors to dereference member pointers and return value
? Should this be refactored into using a union
*/

enum class Type {UNINITIALIZED, INT, DOUBLE, CHAR, STRING, BOOL};

class FlexNode {
private:
    //*MetaData
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
    FlexNode() = default;
    FlexNode(const FlexNode &flexNode);

    //*MetaData Accessors
    [[nodiscard]] bool isEmpty() const {return type == Type::UNINITIALIZED;}
    [[nodiscard]] bool is(const Type checkType) const {return type == checkType;}
    [[nodiscard]] Type getType() const {return type;}
    [[nodiscard]] int asInt() const;
    [[nodiscard]] double asDouble() const;
    [[nodiscard]] char asChar() const;
    [[nodiscard]] std::string asString() const;
    [[nodiscard]] bool asBool() const;

    //*Initialize
    bool addInt(const int &intToAdd);
    bool addDouble(const double &doubleToAdd);
    bool addChar(const char &charToAdd);
    bool addString(const std::string &strToAdd);
    bool addBool(const bool &boolToAdd);

    //*Uninitialize
    bool clear() {return resetNode();}

    //*Modify
    bool changeInt(const int &newInt);
    bool changeDouble(const double &newDbl);
    bool changeChar(const char &newChar);
    bool changeString(const std::string &newStr);
    bool changeBool(const bool &newBool);

    //*Overloaded Operators
    FlexNode& operator=(const FlexNode &rhs);
    std::ostream& print(std::ostream& out) const;

    //*Destructor
    ~FlexNode();
};

std::ostream& operator<<(std::ostream& out, const FlexNode &flexNode);
