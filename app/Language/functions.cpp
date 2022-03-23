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

class NIntPolyEvaluate : public NIntVal {
public:
    NIntPolyEvaluate(Node* p, Node* x) : p(p), x(x) {}
    ~NIntPolyEvaluate() override = default;

    void evaluate() override {
        if (!p->isEval())
            p->evaluate();
        if (!x->isEval())
            x->evaluate();
        Polynomial<Integer> poly = ((NIntPolyVal*)p)->getPoly();
        value = poly(((NIntVal*)x)->getValue());
        evaluated = true;
    }

private:
    Node* p;
    Node* x;
};

class NRatPolyEvaluate : public NRatVal {
public:
    NRatPolyEvaluate(Node* p, Node* x) : p(p), x(x) {}
    ~NRatPolyEvaluate() override = default;

    void evaluate() override {
        if (!p->isEval())
            p->evaluate();
        if (!x->isEval())
            x->evaluate();
        Polynomial<Rational> poly = ((NRatPolyVal*)p)->getPoly();
        value = poly(((NRatVal*)x)->getValue());
        evaluated = true;
    }

private:
    Node* p;
    Node* x;
};

Node* abs(Node* x) {
    return x->t->abs(x);
}

Node* unmin(Node* x) {
    return x->t->unmin(x);
}

Node* gcd(Node* a, Node* b) {
    if (!a->t->eq(Type::INTEGER) && !b->t->eq(Type::INTEGER))
        throw std::runtime_error("No function matching gcd(" + a->t->toStr() + ", " +
                                b->t->toStr() + ")");
    return new NIntGCD(a, b);       
}

Node* binop(Node* l, Node* r, const std::string& op) {
    return l->t->binop(l, op, r);
}

Node* peval(Node* p, Node* x) {
    // Type base = ((NPolyVal*)p)->getBase();
    // if (base == Type::INTEGER && x->type == Type::INTEGER)
    //     return new NIntPolyEvaluate(p, x);
    // else if (base == Type::RATIONAL && x->type == Type::RATIONAL)
    //     return new NRatPolyEvaluate(p, x);
    // else
    //     throw std::runtime_error("No method matching eval(POLYNOMIAL{" + std::string(base) + "}, " + 
    //                             std::string(x->type) + ")");
    return nullptr;
}

Node* assign(std::string name, Node* val) {
    return val->t->assign(name, val);
}

Node* polymono(Node* c, Node* m) {
    return c->t->polyMono(c, m);
}