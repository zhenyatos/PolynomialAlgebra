#include "Interpreter.hpp"

std::map<std::string, Integer> Interpreter::intVars;
std::map<std::string, Rational> Interpreter::ratVars;
std::map<std::string, Modular> Interpreter::modVars;
std::map<std::string, Polynomial<Integer>> Interpreter::intPolyVars;
std::map<std::string, Polynomial<Rational>> Interpreter::ratPolyVars;

std::map<std::string, const TType*> vars;

std::pair<bool, Type> Interpreter::variableExists(const std::string& name) {
    if (intVars.find(name) != intVars.end())
        return { true, Type::INTEGER };
    else if (ratVars.find(name) != ratVars.end())
        return { true, Type::RATIONAL };
    else if (modVars.find(name) != modVars.end())
        return { true, Type::MODULAR };
    else if (intPolyVars.find(name) != intPolyVars.end())
        return { true, Type::POLY_INT };
    else if (ratPolyVars.find(name) != ratPolyVars.end())
        return { true, Type::POLY_RAT };
    return {false, Type::NOTHING };
}

const TType* Interpreter::varEx(const std::string& name) {
    if (vars.find(name) != vars.end())
        return vars[name];
    else
        return nullptr;
}

void Interpreter::setIntValue(const std::string& name, Integer value) {
    const TType* type = varEx(name);
    if (type != nullptr && !type->eq(TType::INTEGER))
        type->erase(name);
    intVars[name] = value;
    vars[name] = TType::INTEGER;
}

void Interpreter::setRatValue(const std::string& name, Rational value) {
    const TType* type = varEx(name);
    if (type != nullptr && !type->eq(TType::RATIONAL))
        type->erase(name);
    ratVars[name] = value;
    vars[name] = TType::RATIONAL;
}

void Interpreter::setModValue(const std::string& name, Modular value) {
    const TType* type = varEx(name);
    if (type != nullptr && !type->eq(TType::MODULAR))
        type->erase(name);
    modVars[name] = value;
    vars[name] = TType::MODULAR;
}

void Interpreter::setPolyIntValue(const std::string& name, const Polynomial<Integer>& value) {
    auto check = variableExists(name);
    if (check.second == Type::INTEGER)
        intVars.erase(name);
    else if (check.second == Type::RATIONAL)
        ratVars.erase(name);
    else if (check.second == Type::MODULAR)
        modVars.erase(name);
    else if (check.second == Type::POLY_RAT)
        ratPolyVars.erase(name);
    intPolyVars[name] = value;
    
}

void Interpreter::setPolyRatValue(const std::string& name, const Polynomial<Rational>& value) {
    auto check = variableExists(name);
    if (check.second == Type::INTEGER)
        intVars.erase(name);
    else if (check.second == Type::RATIONAL)
        ratVars.erase(name);
    else if (check.second == Type::MODULAR)
        modVars.erase(name);
    else if (check.second == Type::POLY_INT)
        intPolyVars.erase(name);
    ratPolyVars[name] = value;
}

Integer Interpreter::getIntValue(const std::string& name) {
    const TType* check = varEx(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::INTEGER))
        throw std::runtime_error("Variable " + name + " is not " + TType::INTEGER->toStr());
    return intVars[name];
}

Rational Interpreter::getRatValue(const std::string& name) {
    const TType* check = varEx(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::RATIONAL))
        throw std::runtime_error("Variable " + name + " is not " + TType::RATIONAL->toStr());
    return ratVars[name];
}

Modular Interpreter::getModValue(const std::string& name) {
    const TType* check = varEx(name);
    if (check == nullptr)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (!check->eq(TType::MODULAR))
        throw std::runtime_error("Variable " + name + " is not " + TType::MODULAR->toStr());
    return modVars[name];
}

Polynomial<Integer> Interpreter::getPolyIntValue(const std::string& name) {
    auto check = variableExists(name);
    if (!check.first)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (check.second != Type::POLY_INT)
        throw std::runtime_error("Variable " + name + " is not POLYNOMIAL(INTEGER)");
    return intPolyVars[name];
}

Polynomial<Rational> Interpreter::getPolyRatValue(const std::string& name) {
    auto check = variableExists(name);
    if (!check.first)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (check.second != Type::POLY_RAT)
        throw std::runtime_error("Variable " + name + " is not POLYNOMIAL(RATIONAL)");
    return ratPolyVars[name];
}