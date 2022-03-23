#include "Interpreter.hpp"

std::map<std::string, const Type*> Interpreter::vars;
std::map<std::string, Integer> Interpreter::intVars;
std::map<std::string, Rational> Interpreter::ratVars;
std::map<std::string, Modular> Interpreter::modVars;
std::map<std::string, Polynomial<Integer>> Interpreter::intPolyVars;
std::map<std::string, Polynomial<Rational>> Interpreter::ratPolyVars;
std::map<std::string, Polynomial<Modular>> Interpreter::modPolyVars;

const Type* Interpreter::variableExist(const std::string& name) {
    if (vars.find(name) != vars.end())
        return vars[name];
    else
        return nullptr;
}

void Interpreter::setIntValue(const std::string& name, Integer value) {
    const Type* type = variableExist(name);
    if (type != nullptr && !type->eq(Type::INTEGER))
        type->erase(name);
    intVars[name] = value;
    vars[name] = Type::INTEGER;
}

void Interpreter::setRatValue(const std::string& name, Rational value) {
    const Type* type = variableExist(name);
    if (type != nullptr && !type->eq(Type::RATIONAL))
        type->erase(name);
    ratVars[name] = value;
    vars[name] = Type::RATIONAL;
}

void Interpreter::setModValue(const std::string& name, Modular value) {
    const Type* type = variableExist(name);
    if (type != nullptr && !type->eq(Type::MODULAR))
        type->erase(name);
    modVars[name] = value;
    vars[name] = Type::MODULAR;
}

void Interpreter::setPolyIntValue(const std::string& name, const Polynomial<Integer>& value) {
    const Type* type = variableExist(name);
    if (type != nullptr && !type->eq(Type::POLY_INT))
        type->erase(name);
    intPolyVars[name] = value; 
    vars[name] = Type::POLY_INT;   
}

void Interpreter::setPolyRatValue(const std::string& name, const Polynomial<Rational>& value) {
    const Type* type = variableExist(name);
    if (type != nullptr && !type->eq(Type::POLY_RAT))
        type->erase(name);
    ratPolyVars[name] = value; 
    vars[name] = Type::POLY_RAT;   
}

void Interpreter::setPolyModValue(const std::string& name, const Polynomial<Modular>& value) {
    const Type* type = variableExist(name);
    if (type != nullptr && !type->eq(Type::POLY_MOD))
        type->erase(name);
    modPolyVars[name] = value; 
    vars[name] = Type::POLY_MOD;   
}

Integer Interpreter::getIntValue(const std::string& name) {
    const Type* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(Type::INTEGER))
        throw std::runtime_error("Variable " + name + " is not " + Type::INTEGER->toStr());
    return intVars[name];
}

Rational Interpreter::getRatValue(const std::string& name) {
    const Type* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(Type::RATIONAL))
        throw std::runtime_error("Variable " + name + " is not " + Type::RATIONAL->toStr());
    return ratVars[name];
}

Modular Interpreter::getModValue(const std::string& name) {
    const Type* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(Type::MODULAR))
        throw std::runtime_error("Variable " + name + " is not " + Type::MODULAR->toStr());
    return modVars[name];
}

Polynomial<Integer> Interpreter::getPolyIntValue(const std::string& name) {
    const Type* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(Type::POLY_INT))
        throw std::runtime_error("Variable " + name + " is not " + Type::POLY_INT->toStr());
    return intPolyVars[name];
}

Polynomial<Rational> Interpreter::getPolyRatValue(const std::string& name) {
    const Type* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(Type::POLY_RAT))
        throw std::runtime_error("Variable " + name + " is not " + Type::POLY_RAT->toStr());
    return ratPolyVars[name];
}

Polynomial<Modular> Interpreter::getPolyModValue(const std::string& name) {
    const Type* check = variableExist(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(Type::POLY_MOD))
        throw std::runtime_error("Variable " + name + " is not " + Type::POLY_MOD->toStr());
    return modPolyVars[name];
}