#include "ModularError.hpp"

ModularError::ModularError(size_t code) 
        : code(code)
    {};

const ModularError ModularError::NONPOSITIVE_MODULUS = ModularError(0);
const ModularError ModularError::NO_INVERSE = ModularError(1);

const char* ModularError::message[2] = {
        "Modulus must be a positive integer",
        "The number has no multiplicative inverse by this modulus"
    };