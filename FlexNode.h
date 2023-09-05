#pragma once

#include <string>
#include <iostream>

enum class Type {UNINITIALIZED, INT, DOUBLE, CHAR, STRING, BOOL};

class FlexNode {
private:
    int* num = nullptr;
    double* dbl = nullptr;
    char* ch = nullptr;
    bool* boo = nullptr;

    bool containsData = false;
    Type type = Type::UNINITIALIZED;

    bool addData(Type newType) {
        if(containsData) {return false;}
        containsData = true;
        type = newType;
        return true;
    }

    bool removeData() {
        if(!containsData) {return false;}
        switch(type) {
            case Type::INT: delete num; break;
            case Type::DOUBLE: delete dbl; break;
            case Type::CHAR: delete ch; break;
            case Type::STRING: delete[] ch; break;
            case Type::BOOL: delete boo; break;
            default: return false;                
        }
        type = Type::UNINITIALIZED;
        containsData = false;
        return true;
    }

public:
    FlexNode() {}
    bool isEmpty() {return !containsData;}
    bool addInt(int intToAdd) {
        if(!addData(Type::INT)) {return false;}
        num = new int(intToAdd);
        return true;
    }
    bool addDouble(double doubleToAdd) {
        if(!addData(Type::DOUBLE)) {return false;}
        dbl = new double(doubleToAdd);
        return true;
    }
    bool addChar(char charToAdd) {
        if(!addData(Type::CHAR)) {return false;}
        ch = new char(charToAdd);
        return true;
    }
    bool addString(std::string strToAdd) {
        if(!addData(Type::STRING)) {return false;}
        ch = new char[strToAdd.length() + 1];
        copy(strToAdd.begin(), strToAdd.end(), ch);
        ch[strToAdd.length()] = '\0';
        return true;
    }
    bool addBool(bool boolToAdd) {
        if(!addData(Type::BOOL)) {return false;}
        boo = new bool(boolToAdd);
        return true;
    }
    bool clear() {return removeData();}

    bool changeInt(int newInt) {
        if(type != Type::INT) {return false;}
        if(!removeData()) {return false;}
        return addInt(newInt);
    }

    bool changeDouble(double newDbl) {
        if(type != Type::DOUBLE) {return false;}
        if(!removeData()) {return false;}
        return addDouble(newDbl);
    }

    bool changeChar(char newChar) {
        if(type != Type::CHAR) {return false;}
        if(!removeData()) {return false;}
        return addChar(newChar);
    }

    bool changeString(std::string newStr) {
        if(type != Type::STRING) {return false;}
        if(!removeData()) {return false;}
        return addString(newStr);
    }
    
    bool changeBool(bool newBool) {
        if(type != Type::BOOL) {return false;}
        if(!removeData()) {return false;}
        return addBool(newBool);
    }

    Type getType() {return type;}

    std::ostream& print(std::ostream& out) {
        switch(type) {
            case Type::INT: return out << *num; break;
            case Type::DOUBLE: return out << *dbl; break;
            case Type::CHAR: return out << *ch; break;
            case Type::STRING: return out << *ch; break;
            case Type::BOOL: return out << *boo; break;
            default: return out;
        }
    }
};

std::ostream& operator<<(std::ostream& out, FlexNode flexNode) {
    return flexNode.print(out);
}