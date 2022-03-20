#pragma once
#include <cstddef>

class Type {
public:
    Type(size_t code = 0) 
        : code(code)
    {};

    // Basic types
    static const Type NOTHING;
    static const Type INTEGER;
    static const Type RATIONAL;
    static const Type MODULAR;
    // Parser-only types
    static const Type VARIABLE;
    static const Type MONOMIAL;
    static const Type POLYNOMIAL;
    // Interpreter-only types
    static const Type POLY_INT;
    static const Type POLY_RAT;
    static const Type POLY_MOD;

    bool operator==(const Type& other) const { return code == other.code; }
    bool operator!=(const Type& other) const { return code != other.code; }

    constexpr operator const char*() {
        return message[code];
    };

private:
    size_t code;

    static const char* message[10];
};