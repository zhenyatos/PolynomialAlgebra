#pragma once
#include "Integer.hpp"
#include "Type.hpp"
#include "Rational.hpp"
#include <string>
#include <map>

class Interpreter {
private:
    static std::map<std::string, Integer> intVars;
    static std::map<std::string, Rational> ratVars;

public:
    Interpreter() = delete;

    static std::pair<bool, Type> variableExists(const std::string& name);
    
    static void setIntValue(const std::string& name, Integer value);

    static void setRatValue(const std::string& name, Rational value);

    static Integer getIntValue(const std::string& name);

    static Rational getRatValue(const std::string& name);
};