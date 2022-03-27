#include "polynodes.hpp"
#include "Interpreter.hpp"

Polynomial<Rational> cast(const Polynomial<Integer>& p) {
    std::vector<Integer> c = p.getCoeff();
    std::vector<Rational> resCoeff;
    for (Integer i : c) {
        resCoeff.push_back(Rational(i));
    }
    return resCoeff;
}

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
    if (left->t->eq(Type::INTEGER))
        a = Polynomial<Integer>({((NIntVal*)left)->getValue()});
    else
        a = ((NIntPolyVal*)left)->getPoly();
    if (right->t->eq(Type::INTEGER))
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
    if (left->t->eq(Type::RATIONAL))
        a = Polynomial<Rational>( { ((NRatVal*)left)->getValue() } );
    else if (left->t->eq(Type::INTEGER))
        a = Polynomial<Rational>( { Rational(((NIntVal*)left)->getValue()) });
    else if (left->t->eq(Type::POLY_RAT))
        a = ((NRatPolyVal*)left)->getPoly();
    else if (left->t->eq(Type::POLY_INT))
        a = cast(((NIntPolyVal*)left)->getPoly());

    if (right->t->eq(Type::RATIONAL))
        b = Polynomial<Rational>({((NRatVal*)right)->getValue()});
    else if (right->t->eq(Type::INTEGER))
        b = Polynomial<Rational>( { Rational(((NIntVal*)right)->getValue()) });
    else if (right->t->eq(Type::POLY_RAT))
        b = ((NRatPolyVal*)right)->getPoly();
    else if (right->t->eq(Type::POLY_INT))
        b = cast(((NIntPolyVal*)right)->getPoly());


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


NModPolyMono::NModPolyMono(Node* c, Node* m) : c(c), m(m) {}

void NModPolyMono::evaluate() {
    if (!c->isEval())
        c->evaluate();
    if (!m->isEval())
        m->evaluate();
    Modular coeff = ((NModVal*)c)->getValue();
    Integer deg = ((NMonom*)m)->getDeg();
    poly = Polynomial<Modular>::Monomial(coeff, int(deg));
    evaluated = true;
}


NModPolyOp::NModPolyOp(Node* left, const std::string& op, Node* right) 
    : left(left), op(op), right(right)
{}

void NModPolyOp::evaluate() {
    if (!left->isEval())
        left->evaluate();
    if (!right->isEval())
        right->evaluate();
    Polynomial<Modular> a;
    Polynomial<Modular> b;
    if (left->t->eq(Type::MODULAR))
        a = Polynomial<Modular>({((NModVal*)left)->getValue()});
    else
        a = ((NModPolyVal*)left)->getPoly();
    if (right->t->eq(Type::MODULAR))
        b = Polynomial<Modular>({((NModVal*)right)->getValue()});
    else
        b = ((NModPolyVal*)right)->getPoly();

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


NModPolyAssign::NModPolyAssign(const std::string& initializer, Node* value)
    : initializer(initializer), expr(value)
{}

void NModPolyAssign::evaluate() {
    if (!expr->isEval())
        expr->evaluate();
    poly = ((NModPolyVal*)expr)->getPoly();
    Interpreter::setPolyModValue(initializer, poly);
    evaluated = true;
}


NModPolyValVar::NModPolyValVar(const std::string& name) : name(name) {}

void NModPolyValVar::evaluate() {
    poly = Interpreter::getPolyModValue(name);
    evaluated = true;
}