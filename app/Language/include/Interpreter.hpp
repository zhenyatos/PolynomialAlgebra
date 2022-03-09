#pragma once
#include "Integer.hpp"
#include <string>
#include <map>

enum class VarType {
    INTEGER,
};

class Interpreter {
private:
    static std::map<std::string, Integer> intVars;

public:
    static bool variableExists(const std::string& name, VarType type) {
        if (type == VarType::INTEGER) {
            if (intVars.find(name) != intVars.end())
                return true;
            else
                return false;
        }    
    }

    static void createVariable(const std::string& name, VarType type) {
        if (variableExists(name, type))
            throw std::runtime_error("Reference to the uninitialized variable " + name);
        if (type == VarType::INTEGER)
            intVars[name] = 0;
    }

    template<class T>
    static void setValue(const std::string& name, T value, VarType type) {
        if (!variableExists(name, type))
            throw std::runtime_error("Reference to the uninitialized variable " + name);
        if (type == VarType::INTEGER) {
            intVars[name] = value;
        }
    }

    static Integer getIntValue(const std::string& name, VarType type) {
        if (!variableExists(name, type))
            throw std::runtime_error("Reference to the uninitialized variable " + name);
        return intVars[name];
    }
};