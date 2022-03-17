#include "nodes.hpp"
#include "Interpreter.hpp"
#include <iostream>

NInt::NInt(Integer val) { value = val; }

void NInt::evaluate() {
    evaluated = true;
}


NRat::NRat(Node* p, Node* q) : p(p), q(q) {}

void NRat::evaluate() {
    if (p->type != Type::INTEGER)
        throw "BAD";
    if (q->type != Type::INTEGER)
        throw "BAD";
    if (!p->isEval())
        p->evaluate();
    if (!q->isEval())
        q->evaluate();
    value = Rational(((NIntVal*)p)->getValue(), 
                        ((NIntVal*)q)->getValue());
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
    try {
        if (op == "+")
            value = a + b;
        else if (op == "-")
            value = a - b;
        else if (op == "*")
            value = a * b;
        else if (op == "/")
            value = a.div(b);
    } catch (const std::exception& ex) {
        std::cout << c_red << ex.what() << c_white << std::endl;
    }
    
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
    try {
        if (op == "+")
            value = a + b;
        else if (op == "-")
            value = a - b;
        else if (op == "*")
            value = a * b;
        else if (op == "/")
            value = a / b;
    } catch (const std::exception& ex) {
        std::cout << c_red << ex.what() << c_white << std::endl;
    }
    
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
    evaluated = true;
}


NIntAssign::NIntAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NIntAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    value = ((NIntVal*)expr)->getValue();
    try {
        Interpreter::setIntValue(initializer, value);
    } catch (const std::exception& ex) {
        std::cout << c_red << ex.what() << c_white << std::endl;
    }
    evaluated = true;
}


NRatAssign::NRatAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NRatAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    value = ((NRatVal*)expr)->getValue();
    try {
        Interpreter::setRatValue(initializer, value);
    } catch (const std::exception& ex) {
        std::cout << c_red << ex.what() << c_white << std::endl;
    }
    evaluated = true;
}


NIntValVar::NIntValVar(const std::string& name) : name(name) {}

void NIntValVar::evaluate() {
    try {
        value = Interpreter::getIntValue(name);
    } catch(const std::exception& ex) {
        std::cout << c_red << ex.what() << c_white << std::endl;
    }
    evaluated = true;
}


NRatValVar::NRatValVar(const std::string& name) : name(name) {}
void NRatValVar::evaluate() {
    try {
        value = Interpreter::getRatValue(name);
    } catch(const std::exception& ex) {
        std::cout << c_red << ex.what() << c_white << std::endl;
    }
    evaluated = true;
}