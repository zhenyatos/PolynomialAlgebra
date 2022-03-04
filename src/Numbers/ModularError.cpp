#include "ModularError.hpp"

ModularError::ModularError(size_t code) 
        : code(code)
    {};

const ModularError ModularError::NONPOSITIVE_MODULUS = ModularError(0);

const char* ModularError::message[1] = {
        "Modulus must be a positive integer"
    };