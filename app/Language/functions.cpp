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

class NIntPolyUMin : public NIntPolyVal {
public:
    NIntPolyUMin(Node* arg) : arg(arg) {}
    ~NIntPolyUMin() override = default;

    void evaluate() override {
        if (!arg->isEval())
            arg->evaluate();
        poly = -((NIntPolyVal*)arg)->getPoly();
    }

private:
    Node* arg;
};

class NRatPolyUMin : public NRatPolyVal {
public:
    NRatPolyUMin(Node* arg) : arg(arg) {}
    ~NRatPolyUMin() override = default;

    void evaluate() override {
        if (!arg->isEval())
            arg->evaluate();
        poly = -((NRatPolyVal*)arg)->getPoly();
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
    else if (t == Type::POLYNOMIAL) {
        Type b = ((NPolyVal*)x)->getBase();
        if (b == Type::INTEGER)
            return new NIntPolyUMin(x);
        else if (b == Type::RATIONAL)
            return new NRatPolyUMin(x);
    }
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

Node* monomial(Node* l, Node* r) {
    if (l->type == Type::INTEGER)
        return new NIntPolyMono(l, r);
    else if (l->type == Type::RATIONAL)
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
    if (base == Type::RATIONAL && x->type == Type::INTEGER)
        return new NIntPolyEvaluate(p, x);
    else if (base == Type::RATIONAL && x->type == Type::RATIONAL)
        return new NRatPolyEvaluate(p, x);
    else
        throw std::runtime_error("NAD");
}

Node* assign(std::string name, Node* val) {
    Type t = val->type;
    if (t == Type::INTEGER)
        return new NIntAssign(name, val);
    else if (t == Type::RATIONAL)
        return new NRatAssign(name, val);
    else if (t == Type::MODULAR)
        return new NModAssign(name, val);
    else if (t == Type::POLYNOMIAL) {
        Type base = ((NPolyVal*)val)->getBase();
        if (base == Type::INTEGER)
            return new NIntPolyAssign(name, val);
        else if (base == Type::RATIONAL)
            return new NRatPolyAssign(name, val);
        else
            throw std::runtime_error("Can't assign " + std::string(t) + "(" + std::string(base) + ")");
    }
    else
        throw std::runtime_error("Can't assign " + std::string(t));
}

Node* polymono(Node* c, Node* m) {
    Type t = c->type;
    if (t == Type::INTEGER) 
        return new NIntPolyMono(c, m);
    else if (t == Type::RATIONAL)
        return new NRatPolyMono(c, m);
}