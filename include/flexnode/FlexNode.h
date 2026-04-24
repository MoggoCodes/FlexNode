#pragma once

#include <cstddef>
#include <iostream>
#include <new>
#include <string>
/*
todo Define Big 3
todo define accessors to dereference member pointers and return value
? Should this be refactored into using a union
*/

namespace flexnode {

enum class Type {UNINITIALIZED, INT, DOUBLE, CHAR, STRING, BOOL};
enum class SetPolicy {KeepType, Retype};

constexpr std::size_t flexNodeMax(const std::size_t lhs, const std::size_t rhs) {
    return lhs > rhs ? lhs : rhs;
}

class FlexNode {
private:
    //*MetaData
    Type type = Type::UNINITIALIZED;

    static constexpr std::size_t kStorageSize = flexNodeMax(
        flexNodeMax(sizeof(int), sizeof(double)),
        flexNodeMax(flexNodeMax(sizeof(char), sizeof(std::string)), sizeof(bool))
    );
    static constexpr std::size_t kStorageAlign = flexNodeMax(
        flexNodeMax(alignof(int), alignof(double)),
        flexNodeMax(flexNodeMax(alignof(char), alignof(std::string)), alignof(bool))
    );
    alignas(kStorageAlign) std::byte storage[kStorageSize]{};

    [[nodiscard]] bool setType(Type newType) const;
    bool resetNode();
    bool changeInt(const int &newInt);
    bool changeDouble(const double &newDbl);
    bool changeChar(const char &newChar);
    bool changeString(const std::string &newStr);
    bool changeBool(const bool &newBool);

    //*Private Data Accessor
    template <typename T>
    T* storageAs() {
        return std::launder(reinterpret_cast<T*>(storage));
    }

    template <typename T>
    const T* storageAs() const {
        return std::launder(reinterpret_cast<const T*>(storage));
    }

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
    bool setValue(const int &newInt, SetPolicy policy = SetPolicy::Retype);
    bool setValue(const double &newDbl, SetPolicy policy = SetPolicy::Retype);
    bool setValue(const char &newChar, SetPolicy policy = SetPolicy::Retype);
    bool setValue(const std::string &newStr, SetPolicy policy = SetPolicy::Retype);
    bool setValue(const char* newStr, SetPolicy policy = SetPolicy::Retype);
    bool setValue(const bool &newBool, SetPolicy policy = SetPolicy::Retype);

    //*Uninitialize
    bool clear() {return resetNode();}

    //*Overloaded Operators
    FlexNode& operator=(const FlexNode &rhs);
    std::ostream& print(std::ostream& out) const;

    //*Destructor
    ~FlexNode();
};

std::ostream& operator<<(std::ostream& out, const FlexNode &flexNode);

} // namespace flexnode
