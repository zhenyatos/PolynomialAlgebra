#include "Type.hpp"
#include "nodes.hpp"
#include "polynodes.hpp"
#include "Interpreter.hpp"


Node* Type::assign(const std::string& name, Node* val) const {
    throw std::runtime_error("Can't assign " + toStr());
}

Node* Type::polyAssign(const std::string& name, Node* val) const {
    throw std::runtime_error("Polynomial{" + toStr() + "} is undefined");
}

Node* Type::unmin(Node* arg) const {
    throw std::runtime_error("No method matching -(" + arg->t->toStr() + ")");
}

Node* Type::abs(Node* arg) const {
    throw std::runtime_error("No method matching abs(" + arg->t->toStr() + ")");
}

Node* Type::opInt(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* Type::opRat(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* Type::opMod(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* Type::opPoly(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* Type::binop(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                b->t->toStr() + ")");
}

class NIntUMin : public NIntVal {
public:
    NIntUMin(Node* arg) : arg(arg) {}
    virtual ~NIntUMin() override = default;

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
    virtual ~NRatUMin() override = default;

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
        Rational arg;
        if (x->t->eq(Type::INTEGER))
            arg = Rational(((NIntVal*)x)->getValue());
        else
            arg = ((NRatVal*)x)->getValue();
        Polynomial<Rational> poly = ((NRatPolyVal*)p)->getPoly();
        value = poly(arg);
        evaluated = true;
    }

private:
    Node* p;
    Node* x;
};

class NModPolyEvaluate : public NModVal {
public:
    NModPolyEvaluate(Node* p, Node* x) : p(p), x(x) {}
    ~NModPolyEvaluate() override = default;

    void evaluate() override {
        if (!p->isEval())
            p->evaluate();
        if (!x->isEval())
            x->evaluate();
        Polynomial<Modular> poly = ((NModPolyVal*)p)->getPoly();
        value = poly(((NModVal*)x)->getValue());
        evaluated = true;
    }

private:
    Node* p;
    Node* x;
};




class TNothing : public Type {
public:
    TNothing() : Type(0) {}
    ~TNothing() override {}

    void print(Node* expr, std::ostream& stream) const override { }
    
    std::string toStr() const override {
        return "NOTHING";
    }
};


class TInteger : public Type {
public:
    TInteger() : Type(1) {}
    ~TInteger() override {}

    const PolyType* getPT() const override { return Type::POLY_INT->getPT(); }

    Node* assign(const std::string& name, Node* val) const override {
        return new NIntAssign(name, val);
    }

    Node* polyAssign(const std::string& name, Node* val) const override {
        return new NIntPolyAssign(name, val);
    }

    void erase(const std::string& name) const override {
        Interpreter::eraseInt(name);
    }
    
    Node* unmin(Node* arg) const override {
        return new NIntUMin(arg);
    }

    Node* abs(Node* arg) const override {
        return new NIntAbs(arg);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        stream << ((NIntVal*)expr)->getValue() << std::endl;
    }

    Node* var(const std::string& name) const override {
        return new NIntValVar(name);
    }

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opInt(a, op, b);
    }

    Node* opInt(Node* a, const std::string& op, Node* b) const override {
        return new NIntOp(a, op, b);
    }
    
    Node* opRat(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
    }

    Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        if (b->t->eq(Type::POLY_INT))
            return new NIntPolyOp(a, op, b);
        else
            return new NRatPolyOp(a, op, b);
    }
    
    std::string toStr() const override {
        return "INTEGER";
    }
};

class TRational : public Type {
public:
    TRational() : Type(2) {}
    ~TRational() override {}

    Node* assign(const std::string& name, Node* val) const override {
        return new NRatAssign(name, val);
    }

    Node* polyAssign(const std::string& name, Node* val) const override {
        return new NRatPolyAssign(name, val);
    }

    void erase(const std::string& name) const override {
        Interpreter::eraseRat(name);
    }

    Node* unmin(Node* arg) const override {
        return new NRatUMin(arg);
    }

    Node* abs(Node* arg) const override {
        return new NRatAbs(arg);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        stream << ((NRatVal*)expr)->getValue() << std::endl;
    }

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opRat(a, op, b);
    }

    Node* var(const std::string& name) const override {
        return new NRatValVar(name);
    }

    Node* opInt(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
    }
    
    Node* opRat(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
    }

    Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        return new NRatPolyOp(a, op, b);
    }

    std::string toStr() const override {
        return "RATIONAL";
    }
};

class TModular : public Type {
public:
    TModular() : Type(3) {}
    ~TModular() override {}

    Node* assign(const std::string& name, Node* val) const override {
        return new NModAssign(name, val);
    }

    Node* polyAssign(const std::string& name, Node* val) const override {
        return new NModPolyAssign(name, val);
    }

    virtual void erase(const std::string& name) const override {
        Interpreter::eraseMod(name);
    }

    Node* unmin(Node* arg) const override {
        return new NModUMin(arg);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        stream << ((NModVal*)expr)->getValue() << std::endl;
    }

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opMod(a, op, b);
    }

    Node* var(const std::string& name) const override {
        return new NModValVar(name);
    }

    Node* opMod(Node* a, const std::string& op, Node* b) const override {
        return new NModOp(a, op, b);
    }

    Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        return new NModPolyOp(a, op, b);
    }

    std::string toStr() const override {
        return "MODULAR";
    }
};

class TVariable : public Type {
public:
    TVariable() : Type(4) {}
    ~TVariable() override {}

    Node* assign(const std::string& name, Node* val) const override {
        throw std::runtime_error("Can't assign " + toStr());
    }
    
    Node* val(Node* arg) const override {
        std::string name = ((NVar*)arg)->getName();
        const Type* type = Interpreter::variableExist(name);
        if (type == nullptr)
            throw std::runtime_error("Reference to the uninitialized variable " + name);
        return type->var(name);
    }

    void print(Node* expr, std::ostream& stream) const override {}
    
    std::string toStr() const override {
        return "VARIABLE";
    }
};

class TMonomial : public Type {
public:
    TMonomial() : Type(5) {}
    ~TMonomial() override {}

    void print(Node* expr, std::ostream& stream) const override { 
        NMonom* m = (NMonom*)expr;
        stream << "X^" << m->getDeg() << std::endl;
    }
    
    std::string toStr() const override {
        return "MONOMIAL";
    }
};

class PTInteger : public PolyType {
public:
    PTInteger() : PolyType(Type::INTEGER) {}
    ~PTInteger() override = default;

    Node* var(const std::string& name) const override {
        return new NIntPolyValVar(name);
    }

    Node* mono(Node* c, Node* m) const override {
        return new NIntPolyMono(c, m);
    }

    Node* eval(Node* p, Node* x) const override {
        return new NIntPolyEvaluate(p, x);
    }
};

class TPolynomial : public Type {
public:
    TPolynomial(const PolyType* pt) : Type(6), pt(pt) {}
    ~TPolynomial() override { delete pt; }

    const PolyType* getPT() const override { return pt; }

    Node* assign(const std::string& name, Node* val) const override {
        return pt->getBase()->polyAssign(name, val);
    }

    bool eq(const Type* other) const override { 
        if (Type::eq(other))
            return pt->getBase() == ((TPolynomial*)other)->pt->getBase();
        return false;
    }

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opPoly(a, op, b);
    }

    Node* opInt(Node* a, const std::string& op, Node* b) const override {
        if (pt->getBase()->eq(Type::INTEGER))
            return new NIntPolyOp(a, op, b);
        else
            return new NRatPolyOp(a, op, b);
    }

    Node* opRat(Node* a, const std::string& op, Node* b) const override {
        return new NRatPolyOp(a, op, b);
    }

    Node* opMod(Node* a, const std::string& op, Node* b) const override {
        return new NModPolyOp(a, op, b);
    }

    Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        return pt->getBase()->opPoly(a, op, b);
    }

    Node* var(const std::string& name) const override {
        return pt->var(name);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        const Type* b = ((TPolynomial*)expr->t)->pt->getBase();
        if (b->eq(Type::INTEGER))
            stream << ((NIntPolyVal*)expr)->getPoly() << std::endl;
        else if (b->eq(Type::RATIONAL))
            stream << ((NRatPolyVal*)expr)->getPoly() << std::endl;
        else if (b->eq(Type::MODULAR))
            stream << ((NModPolyVal*)expr)->getPoly() << std::endl;
    }
    
    std::string toStr() const override {
        return "POLYNOMIAL{" + pt->getBase()->toStr() + "}";
    }

private:
    const PolyType* pt;
};

void Type::initialize() {
    Type::NOTHING    = new TNothing();
    Type::INTEGER    = new TInteger();
    Type::RATIONAL   = new TRational();
    Type::MODULAR    = new TModular();
    Type::VARIABLE   = new TVariable();
    Type::MONOMIAL   = new TMonomial();
    Type::POLY_INT   = new TPolynomial(new PTInteger());
    Type::POLY_RAT   = new TPolynomial(nullptr);
    Type::POLY_MOD   = new TPolynomial(nullptr);
}

void Type::destroy() {
    delete Type::NOTHING;
    delete Type::INTEGER;
    delete Type::RATIONAL;
    delete Type::MODULAR;
    delete Type::VARIABLE;
    delete Type::MONOMIAL;
    delete Type::POLY_INT;
    delete Type::POLY_RAT;
    delete Type::POLY_MOD;
}

const Type* Type::NOTHING  = nullptr;
const Type* Type::INTEGER  = nullptr;
const Type* Type::RATIONAL = nullptr;
const Type* Type::MODULAR  = nullptr;
const Type* Type::VARIABLE = nullptr;
const Type* Type::MONOMIAL = nullptr;
const Type* Type::POLY_INT = nullptr;
const Type* Type::POLY_RAT = nullptr;
const Type* Type::POLY_MOD = nullptr;