#include "nodes.hpp"
#include "polynodes.hpp"
#include "Interpreter.hpp"
#include <iostream>

Node* NVar::value() {
    auto check = Interpreter::variableExists(name);
    if (!check.first)
        throw std::runtime_error("Reference to the uninitialized variable " + name);
    if (check.second == Type::INTEGER)
        return new NIntValVar(name);
    else if (check.second == Type::RATIONAL)
        return new NRatValVar(name);
    else if (check.second == Type::MODULAR)
        return new NModValVar(name);
    else if (check.second == Type::POLY_INT)
        return new NIntPolyValVar(name);
}

NInt::NInt(Integer val) { value = val; }

void NInt::evaluate() {
    evaluated = true;
}


NRat::NRat(Node* p, Node* q) : p(p), q(q) {}

void NRat::evaluate() {
    if (p->type != Type::INTEGER || q->type != Type::INTEGER)
        throw std::runtime_error("No method matching //(" + std::string(p->type) + ", " + std::string(q->type) + ")");
    if (!p->isEval())
        p->evaluate();
    if (!q->isEval())
        q->evaluate();
    value = Rational(((NIntVal*)p)->getValue(), 
                        ((NIntVal*)q)->getValue());
    evaluated = true;
}

NMod::NMod(Node* a, Node* N) : a(a), N(N) {}

void NMod::evaluate() {
    if (a->type != Type::INTEGER || N->type != Type::INTEGER)
        throw std::runtime_error("No method matching [" + std::string(a->type) + ", " + std::string(N->type) + "]");
    if (!a->isEval())
        a->evaluate();
    if (!N->isEval())
        N->evaluate();
    value = Modular(((NIntVal*)a)->getValue(), 
                        ((NIntVal*)N)->getValue());
    evaluated = true;
}


NIntOp::NIntOp(Node* left, const std::string& op, Node* right)
    : left(left), op(op), right(right)
{}

void NIntOp::evaluate() {
    if (!left->isEval())
        left->evaluate();
    if (!right->isEval())
        right->evaluate();
    Integer a = ((NIntVal*)left)->getValue();
    Integer b = ((NIntVal*)right)->getValue();
    if (op == "+")
        value = a + b;
    else if (op == "-")
        value = a - b;
    else if (op == "*")
        value = a * b;
    else if (op == "/")
        value = a.div(b);
    
    evaluated = true;
}


NRatOp::NRatOp(Node* left, const std::string& op, Node* right)
    : left(left), op(op), right(right)
{}

void NRatOp::evaluate() {
    if (!left->isEval())
        left->evaluate();
    if (!right->isEval())
        right->evaluate();
    Rational a;
    Rational b;
    if (left->type == Type::INTEGER)
        a = Rational(((NIntVal*)left)->getValue());
    else
        a = ((NRatVal*)left)->getValue();
    if (right->type == Type::INTEGER)
        b = Rational(((NIntVal*)right)->getValue());
    else
        b = ((NRatVal*)right)->getValue();
    if (op == "+")
        value = a + b;
    else if (op == "-")
        value = a - b;
    else if (op == "*")
        value = a * b;
    else if (op == "/")
        value = a / b;
    
    evaluated = true;
}


NModOp::NModOp(Node* left, const std::string& op, Node* right)
    : left(left), op(op), right(right)
{}

void NModOp::evaluate() {
    if (!left->isEval())
        left->evaluate();
    if (!right->isEval())
        right->evaluate();
    Modular a;
    Modular b;
    if (left->type == Type::MODULAR)
        a = ((NModVal*)left)->getValue();
    if (right->type == Type::MODULAR)
        b = ((NModVal*)right)->getValue();
    if (op == "+")
        value = a + b;
    else if (op == "-")
        value = a - b;
    else if (op == "*")
        value = a * b;
    else if (op == "/")
        value = a / b;
    
    evaluated = true;
}


NPrint::NPrint(Node* value)
    : Node(Type::NOTHING), expr(value)
{}

void NPrint::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    Type type = expr->type;
    if (type == Type::INTEGER)
        std::cout << ((NIntVal*)expr)->getValue() << std::endl;
    else if (type == Type::RATIONAL)
        std::cout << ((NRatVal*)expr)->getValue() << std::endl;
    else if (type == Type::MODULAR)
        std::cout << ((NModVal*)expr)->getValue() << std::endl;
    else if (type == Type::MONOMIAL) {
        NMonom* m = (NMonom*)expr;
        std::cout << "X^" << m->getDeg() << std::endl;
    } 
    else if (type == Type::POLYNOMIAL) {
        Type b = ((NPolyVal*)expr)->getBase();
        if (b == Type::INTEGER) {
            std::cout << ((NIntPolyVal*)expr)->getPoly() << std::endl;
        }
    }
    evaluated = true;
}


NIntAssign::NIntAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NIntAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    value = ((NIntVal*)expr)->getValue();
    Interpreter::setIntValue(initializer, value);
    evaluated = true;
}


NRatAssign::NRatAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NRatAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    value = ((NRatVal*)expr)->getValue();
    Interpreter::setRatValue(initializer, value);
    evaluated = true;
}


NModAssign::NModAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NModAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    value = ((NModVal*)expr)->getValue();
    Interpreter::setModValue(initializer, value);
    evaluated = true;
}


NIntValVar::NIntValVar(const std::string& name) : name(name) {}

void NIntValVar::evaluate() {
    value = Interpreter::getIntValue(name);
    evaluated = true;
}


NRatValVar::NRatValVar(const std::string& name) : name(name) {}

void NRatValVar::evaluate() {
    value = Interpreter::getRatValue(name);
    evaluated = true;
}


NModValVar::NModValVar(const std::string& name) : name(name) {}

void NModValVar::evaluate() {
    value = Interpreter::getModValue(name);
    evaluated = true;
}


