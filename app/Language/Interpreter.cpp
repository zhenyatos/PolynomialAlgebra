#include "Interpreter.hpp"

std::map<std::string, Integer> Interpreter::intVars;
std::map<std::string, Rational> Interpreter::ratVars;

std::pair<bool, Type> Interpreter::variableExists(const std::string& name) {
    if (intVars.find(name) != intVars.end())
        return { true, Type::INTEGER };
    else if (ratVars.find(name) != ratVars.end())
        return { true, Type::RATIONAL };
    else
        return {false, Type::NOTHING };
}

void Interpreter::setIntValue(const std::string& name, Integer value) {
    auto check = variableExists(name);
    if (check.second == Type::RATIONAL) {
        ratVars.erase(name);
    }
    intVars[name] = value;
}

void Interpreter::setRatValue(const std::string& name, Rational value) {
    auto check = variableExists(name);
    if (check.second == Type::INTEGER) {
        intVars.erase(name);
    }
    ratVars[name] = value;
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