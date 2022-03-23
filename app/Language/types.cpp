#include "Type.hpp"
#include "nodes.hpp"
#include "polynodes.hpp"
#include "Interpreter.hpp"

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

class TNothing : public Type {
public:
    TNothing() : Type(0) {}
    virtual ~TNothing() override {}

    Node* assign(const std::string& name, Node* val) const override {
        throw std::runtime_error("Can't assign " + toStr());
    }

    void print(Node* expr, std::ostream& stream) const override { }
    
    virtual std::string toStr() const override {
        return "NOTHING";
    }
};


class TInteger : public Type {
public:
    TInteger() : Type(1) {}
    virtual ~TInteger() override {}

    Node* assign(const std::string& name, Node* val) const override {
        return new NIntAssign(name, val);
    }

    Node* polyAssign(const std::string& name, Node* val) const override {
        return new NIntPolyAssign(name, val);
    }

    virtual void erase(const std::string& name) const override {
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

    virtual Node* var(const std::string& name) const override {
        return new NIntValVar(name);
    }

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opInt(a, op, b);
    }

    virtual Node* opInt(Node* a, const std::string& op, Node* b) const override {
        return new NIntOp(a, op, b);
    }
    
    virtual Node* opRat(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
    }

    virtual Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        return new NIntPolyOp(a, op, b);
    }

    virtual Node* polyVar(const std::string& name) const { return 
        new NIntPolyValVar(name); 
    }

    virtual Node* polyMono(Node* c, Node* m) const override {
        return new NIntPolyMono(c, m);
    }
    
    virtual std::string toStr() const override {
        return "INTEGER";
    }
};

class TRational : public Type {
public:
    TRational() : Type(2) {}
    virtual ~TRational() override {}

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

    virtual Node* var(const std::string& name) const override {
        return new NRatValVar(name);
    }

    virtual Node* polyVar(const std::string& name) const override {
        return new NRatPolyValVar(name);
    }

    virtual Node* opInt(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
    }
    
    virtual Node* opRat(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
    }

    virtual Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        return new NRatPolyOp(a, op, b);
    }

    virtual Node* polyMono(Node* c, Node* m) const override {
        return new NRatPolyMono(c, m);
    }

    virtual std::string toStr() const override {
        return "RATIONAL";
    }
};

class TModular : public Type {
public:
    TModular() : Type(3) {}
    virtual ~TModular() override {}

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

    virtual Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opMod(a, op, b);
    }

    virtual Node* var(const std::string& name) const override {
        return new NModValVar(name);
    }

    virtual Node* polyVar(const std::string& name) const override {
        return new NModPolyValVar(name);
    }

    virtual Node* opMod(Node* a, const std::string& op, Node* b) const override {
        return new NModOp(a, op, b);
    }

    virtual Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        return new NModPolyOp(a, op, b);
    }

    virtual Node* polyMono(Node* c, Node* m) const override {
        return new NModPolyMono(c, m);
    }

    virtual std::string toStr() const override {
        return "MODULAR";
    }
};

class TVariable : public Type {
public:
    TVariable() : Type(4) {}
    virtual ~TVariable() override {}

    Node* assign(const std::string& name, Node* val) const override {
        throw std::runtime_error("Can't assign " + toStr());
    }
    
    virtual Node* val(Node* arg) const override {
        std::string name = ((NVar*)arg)->getName();
        const Type* type = Interpreter::variableExist(name);
        if (type == nullptr)
            throw std::runtime_error("Reference to the uninitialized variable " + name);
        return type->var(name);
    }

    void print(Node* expr, std::ostream& stream) const override {}
    
    virtual std::string toStr() const override {
        return "VARIABLE";
    }
};

class TMonomial : public Type {
public:
    TMonomial() : Type(5) {}
    virtual ~TMonomial() override {}

    Node* assign(const std::string& name, Node* val) const override {
        throw std::runtime_error("Can't assign " + toStr());
    }
    
    void print(Node* expr, std::ostream& stream) const override { 
        NMonom* m = (NMonom*)expr;
        stream << "X^" << m->getDeg() << std::endl;
    }
    
    virtual std::string toStr() const override {
        return "MONOMIAL";
    }
};

class TPolynomial : public Type {
public:
    TPolynomial(const Type* base) : Type(6), base(base) {}
    virtual ~TPolynomial() override {}

    Node* assign(const std::string& name, Node* val) const override {
        return base->polyAssign(name, val);
    }

    virtual bool eq(const Type* other) const override { 
        if (Type::eq(other)) {
            return base == ((TPolynomial*)other)->base;
        }
        return false;
    }

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opPoly(a, op, b);
    }

    virtual Node* opInt(Node* a, const std::string& op, Node* b) const override {
        return new NIntPolyOp(a, op, b);
    }

    virtual Node* opRat(Node* a, const std::string& op, Node* b) const override {
        return new NRatPolyOp(a, op, b);
    }

    virtual Node* opMod(Node* a, const std::string& op, Node* b) const override {
        return new NModPolyOp(a, op, b);
    }

    virtual Node* opPoly(Node* a, const std::string& op, Node* b) const override {
        return base->opPoly(a, op, b);
    }

    virtual Node* var(const std::string& name) const override {
        return base->polyVar(name);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        const Type* b = ((TPolynomial*)expr->t)->base;
        if (b->eq(Type::INTEGER))
            stream << ((NIntPolyVal*)expr)->getPoly() << std::endl;
        else if (b->eq(Type::RATIONAL))
            stream << ((NRatPolyVal*)expr)->getPoly() << std::endl;
        else if (b->eq(Type::MODULAR))
            stream << ((NModPolyVal*)expr)->getPoly() << std::endl;
    }
    
    virtual std::string toStr() const override {
        return "POLYNOMIAL{" + base->toStr() + "}";
    }

    const Type* getBase() const { return base; }

private:
    const Type* base;
};

void Type::initialize() {
    Type::NOTHING    = new TNothing();
    Type::INTEGER    = new TInteger();
    Type::RATIONAL   = new TRational();
    Type::MODULAR    = new TModular();
    Type::VARIABLE   = new TVariable();
    Type::MONOMIAL   = new TMonomial();
    Type::POLY_INT   = new TPolynomial(Type::INTEGER);
    Type::POLY_RAT   = new TPolynomial(Type::RATIONAL);
    Type::POLY_MOD   = new TPolynomial(Type::MODULAR);
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