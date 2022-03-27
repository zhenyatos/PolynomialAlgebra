#include "functions.hpp"
#include "polynodes.hpp"

class NIntGCD : public NIntVal {
public:
    NIntGCD(Node* a, Node* b) : a(a), b(b) {}
    ~NIntGCD() override = default;

    void evaluate() override {
        if (!a->isEval())
            a->evaluate();
        if (!b->isEval())
            b->evaluate();
        value = GCD(((NIntVal*)a)->getValue(), ((NIntVal*)b)->getValue());
    }

private:
    Node* a;
    Node* b;
};

class NRatPolyGCD : public NRatPolyVal {
public:
    NRatPolyGCD(Node* a, Node* b) : a(a), b(b) {}
    ~NRatPolyGCD() override = default;

    void evaluate() override {
        if (!a->isEval())
            a->evaluate();
        if (!b->isEval())
            b->evaluate();
        poly = GCD(((NRatPolyVal*)a)->getPoly(), ((NRatPolyVal*)b)->getPoly());
    }

private:
    Node* a;
    Node* b;
};

class NModPolyGCD : public NModPolyVal {
public:
    NModPolyGCD(Node* a, Node* b) : a(a), b(b) {}
    ~NModPolyGCD() override = default;

    void evaluate() override {
        if (!a->isEval())
            a->evaluate();
        if (!b->isEval())
            b->evaluate();
        poly = GCD(((NModPolyVal*)a)->getPoly(), ((NModPolyVal*)b)->getPoly());
    }

private:
    Node* a;
    Node* b;
};

Node* abs(Node* x) {
    return x->t->abs(x);
}

Node* unmin(Node* x) {
    return x->t->unmin(x);
}

Node* gcd(Node* a, Node* b) {
    if (a->t->eq(Type::INTEGER) && b->t->eq(Type::INTEGER))
        return new NIntGCD(a, b);
    if (a->t->eq(Type::POLY_RAT) && b->t->eq(Type::POLY_RAT)) 
        return new NRatPolyGCD(a, b);
    if (a->t->eq(Type::POLY_MOD) && b->t->eq(Type::POLY_MOD))
        return new NModPolyGCD(a, b);
    throw std::runtime_error("No function matching gcd(" + a->t->toStr() + ", " +
                            b->t->toStr() + ")");      
}

Node* binop(Node* l, Node* r, const std::string& op) {
    return l->t->binop(l, op, r);
}

Node* peval(Node* p, Node* x) {
    return p->t->polyEval(p, x);
}

Node* assign(std::string name, Node* val) {
    return val->t->assign(name, val);
}

Node* polymono(Node* c, Node* m) {
    return c->t->polyMono(c, m);
}