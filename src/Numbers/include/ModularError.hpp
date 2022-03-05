#pragma once
#include <iostream>

class ModularError {
public:
    static const ModularError NONPOSITIVE_MODULUS;
    static const ModularError NO_INVERSE;

    constexpr operator const char*() {
        return message[code];
    }

private:
    static const char* message[2];

    size_t code;

    ModularError(size_t code);
};
