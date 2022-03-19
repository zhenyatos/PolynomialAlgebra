#include "ModularError.hpp"

ModularError::ModularError(size_t code) 
        : code(code)
    {};

const ModularError ModularError::NEGATIVE_MODULUS = ModularError(0);
const ModularError ModularError::NO_INVERSE = ModularError(1);
const ModularError ModularError::DIFF_MODULUS = ModularError(2);

const char* ModularError::message[3] = {
        "Modulus must be a positive integer",
        "The number has no multiplicative inverse by this modulus",
        "Operands have different modulus"
    };