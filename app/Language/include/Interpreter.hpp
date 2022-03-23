#pragma once
#include "Integer.hpp"
#include "Type.hpp"
#include "Rational.hpp"
#include "Modular.hpp"
#include "Polynomial.hpp"
#include "nodes.hpp"
#include <string>
#include <map>

class Interpreter {
private:
    static std::map<std::string, const TType*> vars;
    static std::map<std::string, Integer> intVars;
    static std::map<std::string, Rational> ratVars;
    static std::map<std::string, Modular> modVars;
    static std::map<std::string, Polynomial<Integer>> intPolyVars;
    static std::map<std::string, Polynomial<Rational>> ratPolyVars;
    static std::map<std::string, Polynomial<Modular>> modPolyVars;

public:
    Interpreter() = delete;
    
    static const TType* variableExist(const std::string& name);

    static void eraseInt(const std::string& name) {
        intVars.erase(name);
    }

    static void eraseRat(const std::string& name) {
        ratVars.erase(name);
    }

    static void eraseMod(const std::string& name) {
        modVars.erase(name);
    }

    static void erasePolyInt(const std::string& name) {
        intPolyVars.erase(name);
    }

    static void erasePolyRat(const std::string& name) {
        ratPolyVars.erase(name);
    }

    static void erasePolyMod(const std::string& name) {
        modPolyVars.erase(name);
    }
    
    static void setIntValue(const std::string& name, Integer value);

    static void setRatValue(const std::string& name, Rational value);

    static void setModValue(const std::string& name, Modular value);

    static void setPolyIntValue(const std::string& name, const Polynomial<Integer>& value);

    static void setPolyRatValue(const std::string& name, const Polynomial<Rational>& value);
    
    static void setPolyModValue(const std::string& name, const Polynomial<Modular>& value);

    static Integer getIntValue(const std::string& name);

    static Rational getRatValue(const std::string& name);

    static Modular getModValue(const std::string& name);

    static Polynomial<Integer> getPolyIntValue(const std::string& name);

    static Polynomial<Rational> getPolyRatValue(const std::string& name);

    static Polynomial<Modular> getPolyModValue(const std::string& name);
};