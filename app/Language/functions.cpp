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

class NIntUMin : public NIntVal {
public:
    NIntUMin(Node* arg) : arg(arg) {}
    ~NIntUMin() override = default;

    void evaluate() override {
        if (!arg->isEval())
            arg->evaluate();
        value = -((NIntVal*)arg)->getValue();
    }

private:
    Node* arg;
};

class NRatUMin : public NRatVal {
public:
    NRatUMin(Node* arg) : arg(arg) {}
    ~NRatUMin() override = default;

    void evaluate() override {
        if (!arg->isEval())
            arg->evaluate();
        value = -((NRatVal*)arg)->getValue();
    }

private:
    Node* arg;
};

class NModUMin : public NModVal {
public:
    NModUMin(Node* arg) : arg(arg) {}
    ~NModUMin() override = default;

    void evaluate() override {
        if (!arg->isEval())
            arg->evaluate();
        value = -((NModVal*)arg)->getValue();
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

Node* abs(Node* x) {
    Type t = x->type;
    if (t == Type::INTEGER) {
        return new NIntAbs(x);
    } else if (t == Type::RATIONAL) {
        return new NRatAbs(x);
    } else
        throw std::runtime_error("No function matching abs(" + std::string(t) + ")");
}

Node* unmin(Node* x) {
    Type t = x->type;
    if (t == Type::INTEGER) 
        return new NIntUMin(x);
    else if (t == Type::RATIONAL) 
        return new NRatUMin(x);
    else if (t == Type::MODULAR)
        return new NModUMin(x);
    else
        throw std::runtime_error("No function matching -(" + std::string(t) + ")");
}

Node* gcd(Node* a, Node* b) {
    if (a->type != Type::INTEGER || b->type != Type::INTEGER)
        throw std::runtime_error("No function matching gcd(" + std::string(a->type) + ", " +
                                std::string(b->type) + ")");
    return new NIntGCD(a, b);       
}

Node* binop(Node* l, Node* r, const std::string& op) {
    Type ltype = l->type;
    Type rtype = r->type;
    if (ltype == Type::INTEGER && rtype == Type::INTEGER)
        return new NIntOp(l, op, r);
    else if (ltype == Type::RATIONAL && rtype == Type::RATIONAL ||
            ltype == Type::INTEGER && rtype == Type::RATIONAL ||
            ltype == Type::RATIONAL && rtype == Type::INTEGER)
        return new NRatOp(l, op, r);
    else if (ltype == Type::MODULAR && rtype == Type::MODULAR)
        return new NModOp(l, op, r);
    else if (ltype == Type::POLYNOMIAL || rtype == Type::POLYNOMIAL)
        return polyop(l, r, op);
    else
        throw std::runtime_error("No method matching " + op + "(" + std::string(ltype) + ", " +
                                    std::string(rtype) + ")");
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
    else
        throw std::runtime_error("No method matching " + op + "(" + std::string(ltype) + ", " +
                                    std::string(rtype) + ")");
}

Node* monomial(Node* l, Node* r) {
    if (l->type == Type::INTEGER)
        return new NIntPolyMono(l, r);
}

Node* peval(Node* p, Node* x) {
    if (p->type == Type::POLYNOMIAL && x->type == Type::INTEGER)
        return new NIntPolyEvaluate(p, x);
    else
        throw std::runtime_error("NAD");
}