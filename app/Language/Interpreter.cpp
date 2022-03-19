#include "Interpreter.hpp"

std::map<std::string, Integer> Interpreter::intVars;
std::map<std::string, Rational> Interpreter::ratVars;
std::map<std::string, Modular> Interpreter::modVars;
std::map<std::string, Polynomial<Integer>> Interpreter::intPolyVars;

std::pair<bool, Type> Interpreter::variableExists(const std::string& name) {
    if (intVars.find(name) != intVars.end())
        return { true, Type::INTEGER };
    else if (ratVars.find(name) != ratVars.end())
        return { true, Type::RATIONAL };
    else if (modVars.find(name) != modVars.end())
        return { true, Type::MODULAR };
    else if (intPolyVars.find(name) != intPolyVars.end())
        return { true, Type::POLYNOMIAL };
    return {false, Type::NOTHING };
}

void Interpreter::setIntValue(const std::string& name, Integer value) {
    auto check = variableExists(name);
    if (check.second == Type::RATIONAL)
        ratVars.erase(name);
    else if (check.second == Type::MODULAR)
        modVars.erase(name);
    else if (check.second == Type::POLYNOMIAL)
        intPolyVars.erase(name);
    intVars[name] = value;
}

void Interpreter::setRatValue(const std::string& name, Rational value) {
    auto check = variableExists(name);
    if (check.second == Type::INTEGER)
        intVars.erase(name);
    else if (check.second == Type::MODULAR)
        modVars.erase(name);
    else if (check.second == Type::POLYNOMIAL)
        intPolyVars.erase(name);
    ratVars[name] = value;
}

void Interpreter::setModValue(const std::string& name, Modular value) {
    auto check = variableExists(name);
    if (check.second == Type::INTEGER)
        intVars.erase(name);
    else if (check.second == Type::RATIONAL)
        ratVars.erase(name);
    else if (check.second == Type::POLYNOMIAL)
        intPolyVars.erase(name);
    modVars[name] = value;
}

void Interpreter::setPolyIntValue(const std::string& name, const Polynomial<Integer>& value) {
    auto check = variableExists(name);
    if (check.second == Type::INTEGER)
        intVars.erase(name);
    else if (check.second == Type::RATIONAL)
        ratVars.erase(name);
    else if (check.second == Type::MODULAR)
        modVars.erase(name);
    intPolyVars[name] = value;
}

Integer Interpreter::getIntValue(const std::string& name) {
    auto check = variableExists(name);
    if (!check.first)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (check.second != Type::INTEGER)
        throw std::runtime_error("Variable " + name + " is not Integer");
    return intVars[name];
}

Rational Interpreter::getRatValue(const std::string& name) {
    auto check = variableExists(name);
    if (!check.first)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (check.second != Type::RATIONAL)
        throw std::runtime_error("Variable " + name + " is not Rational");
    return ratVars[name];
}

Modular Interpreter::getModValue(const std::string& name) {
    auto check = variableExists(name);
    if (!check.first)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (check.second != Type::MODULAR)
        throw std::runtime_error("Variable " + name + " is not Modular");
    return modVars[name];
}

Polynomial<Integer> Interpreter::getPolyIntValue(const std::string& name) {
    auto check = variableExists(name);
    if (!check.first)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    else if (check.second != Type::POLYNOMIAL)
        throw std::runtime_error("Variable " + name + " is not Polynomial");
    return intPolyVars[name];
}