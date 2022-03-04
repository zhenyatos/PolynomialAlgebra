#pragma once
#include <iostream>

class ModularError {
public:
    static const ModularError NONPOSITIVE_MODULUS;

    constexpr operator const char*() {
        return message[code];
    }

private:
    static const char* message[1];

    size_t code;

    ModularError(size_t code);
};
