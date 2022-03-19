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
    static const Type VARIABLE;
    static const Type MONOMIAL;

    bool operator==(const Type& other) const { return code == other.code; }
    bool operator!=(const Type& other) const { return code != other.code; }

    constexpr operator const char*() {
        return message[code];
    };

private:
    size_t code;

    static const char* message[6];
};