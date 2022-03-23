#include "Interpreter.hpp"

std::map<std::string, Integer> Interpreter::intVars;
std::map<std::string, Rational> Interpreter::ratVars;
std::map<std::string, Modular> Interpreter::modVars;
std::map<std::string, Polynomial<Integer>> Interpreter::intPolyVars;
std::map<std::string, Polynomial<Rational>> Interpreter::ratPolyVars;

std::map<std::string, const TType*> vars;

const TType* Interpreter::variableExist(const std::string& name) {
    if (vars.find(name) != vars.end())
        return vars[name];
    else
        return nullptr;
}

void Interpreter::setIntValue(const std::string& name, Integer value) {
    const TType* type = variableExist(name);
    if (type != nullptr && !type->eq(TType::INTEGER))
        type->erase(name);
    intVars[name] = value;
    vars[name] = TType::INTEGER;
}

void Interpreter::setRatValue(const std::string& name, Rational value) {
    const TType* type = variableExist(name);
    if (type != nullptr && !type->eq(TType::RATIONAL))
        type->erase(name);
    ratVars[name] = value;
    vars[name] = TType::RATIONAL;
}

void Interpreter::setModValue(const std::string& name, Modular value) {
    const TType* type = variableExist(name);
    if (type != nullptr && !type->eq(TType::MODULAR))
        type->erase(name);
    modVars[name] = value;
    vars[name] = TType::MODULAR;
}

void Interpreter::setPolyIntValue(const std::string& name, const Polynomial<Integer>& value) {
    const TType* type = variableExist(name);
    if (type != nullptr && !type->eq(TType::POLY_INT))
        type->erase(name);
    intPolyVars[name] = value; 
    vars[name] = TType::POLY_INT;   
}

void Interpreter::setPolyRatValue(const std::string& name, const Polynomial<Rational>& value) {
    const TType* type = variableExist(name);
    if (type != nullptr && !type->eq(TType::POLY_RAT))
        type->erase(name);
    ratPolyVars[name] = value; 
    vars[name] = TType::POLY_RAT;   
}

Integer Interpreter::getIntValue(const std::string& name) {
    const TType* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::INTEGER))
        throw std::runtime_error("Variable " + name + " is not " + TType::INTEGER->toStr());
    return intVars[name];
}

Rational Interpreter::getRatValue(const std::string& name) {
    const TType* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::RATIONAL))
        throw std::runtime_error("Variable " + name + " is not " + TType::RATIONAL->toStr());
    return ratVars[name];
}

Modular Interpreter::getModValue(const std::string& name) {
    const TType* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::MODULAR))
        throw std::runtime_error("Variable " + name + " is not " + TType::MODULAR->toStr());
    return modVars[name];
}

Polynomial<Integer> Interpreter::getPolyIntValue(const std::string& name) {
    const TType* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::POLY_INT))
        throw std::runtime_error("Variable " + name + " is not " + TType::POLY_INT->toStr());
    return intPolyVars[name];
}

Polynomial<Rational> Interpreter::getPolyRatValue(const std::string& name) {
    const TType* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::POLY_RAT))
        throw std::runtime_error("Variable " + name + " is not " + TType::POLY_RAT->toStr());
    return ratPolyVars[name];
}