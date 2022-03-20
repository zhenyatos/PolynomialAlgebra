#include "polynodes.hpp"
#include "Interpreter.hpp"

NPowMonom::NPowMonom(Node* N) : N(N) {}

void NPowMonom::evaluate() {
    if (!N->isEval())  
        N->evaluate();
    deg = ((NIntVal*)N)->getValue();
    if (deg < 0)
        throw std::runtime_error("The monomial degree is negative");
    evaluated = true;
}


NIntPolyMono::NIntPolyMono(Node* c, Node* m) : c(c), m(m) {}

void NIntPolyMono::evaluate() {
    if (!c->isEval())
        c->evaluate();
    if (!m->isEval())
        m->evaluate();
    Integer coeff = ((NIntVal*)c)->getValue();
    Integer deg = ((NMonom*)m)->getDeg();
    poly = Polynomial<Integer>::Monomial(coeff, int(deg));
    evaluated = true;
}


NIntPolyOp::NIntPolyOp(Node* left, const std::string& op, Node* right) 
    : left(left), op(op), right(right)
{}

void NIntPolyOp::evaluate() {
    if (!left->isEval())
        left->evaluate();
    if (!right->isEval())
        right->evaluate();
    Polynomial<Integer> a;
    Polynomial<Integer> b;
    if (left->type == Type::INTEGER)
        a = Polynomial<Integer>({((NIntVal*)left)->getValue()});
    else
        a = ((NIntPolyVal*)left)->getPoly();
    if (right->type == Type::INTEGER)
        b = Polynomial<Integer>({((NIntVal*)right)->getValue()});
    else
        b = ((NIntPolyVal*)right)->getPoly();

    if (op == "+")
        poly = a + b;
    else if (op == "-")
        poly = a - b;
    else if (op == "*")
        poly = a * b;

    evaluated = true;
}


NIntPolyAssign::NIntPolyAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NIntPolyAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    poly = ((NIntPolyVal*)expr)->getPoly();
    Interpreter::setPolyIntValue(initializer, poly);
    evaluated = true;
}


NIntPolyValVar::NIntPolyValVar(const std::string& name) : name(name) {}

void NIntPolyValVar::evaluate() {
    poly = Interpreter::getPolyIntValue(name);
    evaluated = true;
}


NRatPolyMono::NRatPolyMono(Node* c, Node* m) : c(c), m(m) {}

void NRatPolyMono::evaluate() {
    if (!c->isEval())
        c->evaluate();
    if (!m->isEval())
        m->evaluate();
    Rational coeff = ((NRatVal*)c)->getValue();
    Integer deg = ((NMonom*)m)->getDeg();
    poly = Polynomial<Rational>::Monomial(coeff, int(deg));
    evaluated = true;
}


NRatPolyOp::NRatPolyOp(Node* left, const std::string& op, Node* right) 
    : left(left), op(op), right(right)
{}

void NRatPolyOp::evaluate() {
    if (!left->isEval())
        left->evaluate();
    if (!right->isEval())
        right->evaluate();
    Polynomial<Rational> a;
    Polynomial<Rational> b;
    if (left->type == Type::RATIONAL)
        a = Polynomial<Rational>({((NRatVal*)left)->getValue()});
    else
        a = ((NRatPolyVal*)left)->getPoly();
    if (right->type == Type::RATIONAL)
        b = Polynomial<Rational>({((NRatVal*)right)->getValue()});
    else
        b = ((NRatPolyVal*)right)->getPoly();

    if (op == "+")
        poly = a + b;
    else if (op == "-")
        poly = a - b;
    else if (op == "*")
        poly = a * b;
    else if (op == "/")
        poly = a.div(b);

    evaluated = true;
}


NRatPolyAssign::NRatPolyAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NRatPolyAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    poly = ((NRatPolyVal*)expr)->getPoly();
    Interpreter::setPolyRatValue(initializer, poly);
    evaluated = true;
}


NRatPolyValVar::NRatPolyValVar(const std::string& name) : name(name) {}

void NRatPolyValVar::evaluate() {
    poly = Interpreter::getPolyRatValue(name);
    evaluated = true;
}
