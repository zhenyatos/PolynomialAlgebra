#pragma once
#include <iostream>

class ModularError {
public:
    static const ModularError NEGATIVE_MODULUS;
    static const ModularError NO_INVERSE;
    static const ModularError DIFF_MODULUS;

    constexpr operator const char*() {
        return message[code];
    }

private:
    static const char* message[3];

    size_t code;

    ModularError(size_t code);
};
