#include "functions.hpp"
#include "polynodes.hpp"

class NIntAbs : public NIntVal {
public:
    NIntAbs(Node* arg) : arg(arg) {}
    ~NIntAbs() override = default;

    void evaluate() override {
        if (!arg->isEval())
            arg->evaluate();
        value = ((NIntVal*)arg)->getValue().abs();
    }

private:
    Node* arg;
};

class NRatAbs : public NRatVal {
public:
    NRatAbs(Node* arg) : arg(arg) {}
    ~NRatAbs() override = default;

    void evaluate() override {
        if (!arg->isEval())
            arg->evaluate();
        value = ((NRatVal*)arg)->getValue().abs();
    }

private:
    Node* arg;
};

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
    if (x->t->eq(TType::INTEGER))
        return new NIntAbs(x);
    else if (x->t->eq(TType::RATIONAL))
        return new NRatAbs(x);
    else
        throw std::runtime_error("No function matching abs(" + x->t->toStr() + ")");
}

Node* unmin(Node* x) {
    return x->t->unmin(x);
}

Node* gcd(Node* a, Node* b) {
    if (a->t->eq(TType::INTEGER) && b->t->eq(TType::INTEGER))
        throw std::runtime_error("No function matching gcd(" + std::string(a->type) + ", " +
                                std::string(b->type) + ")");
    return new NIntGCD(a, b);       
}

Node* binop(Node* l, Node* r, const std::string& op) {
    return l->t->binop(l, op, r);
}

Node* monomial(Node* l, Node* r) {
    if (l->t->eq(TType::INTEGER))
        return new NIntPolyMono(l, r);
    else if (l->t->eq(TType::RATIONAL))
        return new NRatPolyMono(l, r);
}

Node* polyop(Node* l, Node* r, const std::string& op) {
    Type ltype;
    Type rtype;
    if (l->type == Type::POLYNOMIAL)
        ltype = ((NPolyVal*)l)->getBase();
    else
        ltype = l->type;
    if (r->type == Type::POLYNOMIAL) 
        rtype = ((NPolyVal*)r)->getBase();
    else 
        rtype = r->type;
    if (ltype == Type::INTEGER && rtype == Type::INTEGER)
        return new NIntPolyOp(l, op, r);
    if (ltype == Type::RATIONAL && rtype == Type::RATIONAL)
        return new NRatPolyOp(l, op, r);
    else
        throw std::runtime_error("No method matching " + op + "(" + std::string(ltype) + ", " +
                                    std::string(rtype) + ")");
}

Node* peval(Node* p, Node* x) {
    Type base = ((NPolyVal*)p)->getBase();
    if (base == Type::INTEGER && x->type == Type::INTEGER)
        return new NIntPolyEvaluate(p, x);
    else if (base == Type::RATIONAL && x->type == Type::RATIONAL)
        return new NRatPolyEvaluate(p, x);
    else
        throw std::runtime_error("No method matching eval(POLYNOMIAL{" + std::string(base) + "}, " + 
                                std::string(x->type) + ")");
}

Node* assign(std::string name, Node* val) {
    return val->t->assign(name, val);
}

Node* polymono(Node* c, Node* m) {
    return c->t->polymono(c, m);
}