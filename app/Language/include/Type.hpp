#pragma once
#include <cstddef>

class Type {
public:
    Type(size_t code = 0) 
        : code(code)
    {};

    static const Type NOTHING;
    static const Type INTEGER;
    static const Type RATIONAL;
    static const Type MODULAR;

    bool operator==(const Type& other) const { return code == other.code; }
    bool operator!=(const Type& other) const { return code != other.code; }

private:
    size_t code;
};