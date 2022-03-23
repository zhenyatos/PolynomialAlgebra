#include "nodes.hpp"
#include "polynodes.hpp"
#include "Interpreter.hpp"

Node* TType::polyAssign(const std::string& name, Node* val) const {
    throw std::runtime_error("Polynomial{" + toStr() + "} is undefined");
}

Node* TType::unmin(Node* arg) const {
    throw std::runtime_error("No function matching -(" + arg->t->toStr() + ")");
}

Node* TType::opInt(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* TType::opRat(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* TType::opMod(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* TType::opPoly(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
}

Node* TType::binop(Node* a, const std::string& op, Node* b) const {
    throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                b->t->toStr() + ")");
}

 

const TType* TType::NOTHING  = nullptr;
const TType* TType::INTEGER  = nullptr;
const TType* TType::RATIONAL = nullptr;
const TType* TType::MODULAR  = nullptr;
const TType* TType::VARIABLE = nullptr;
const TType* TType::MONOMIAL = nullptr;
const TType* TType::POLY_INT = nullptr;
const TType* TType::POLY_RAT = nullptr;
const TType* TType::POLY_MOD = nullptr;

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

class TNothing : public TType {
public:
    TNothing() : TType(0) {}
    virtual ~TNothing() override {}

    Node* assign(const std::string& name, Node* val) const override {
        throw std::runtime_error("Can't assign " + toStr());
    }

    void print(Node* expr, std::ostream& stream) const override { }
    
    virtual std::string toStr() const override {
        return "NOTHING";
    }
};


class TInteger : public TType {
public:
    TInteger() : TType(1) {}
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

class TRational : public TType {
public:
    TRational() : TType(2) {}
    virtual ~TRational() override {}

    Node* assign(const std::string& name, Node* val) const override {
        return new NRatAssign(name, val);
    }

    Node* polyAssign(const std::string& name, Node* val) const override {
        return new NRatPolyAssign(name, val);
    }

    virtual void erase(const std::string& name) const override {
        Interpreter::eraseRat(name);
    }

    Node* unmin(Node* arg) const override {
        return new NRatUMin(arg);
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

class TModular : public TType {
public:
    TModular() : TType(3) {}
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

class TVariable : public TType {
public:
    TVariable() : TType(4) {}
    virtual ~TVariable() override {}

    Node* assign(const std::string& name, Node* val) const override {
        throw std::runtime_error("Can't assign " + toStr());
    }
    
    virtual Node* val(Node* arg) const override {
        std::string name = ((NVar*)arg)->getName();
        const TType* type = Interpreter::variableExist(name);
        if (type == nullptr)
            throw std::runtime_error("Reference to the uninitialized variable " + name);
        return type->var(name);
    }

    void print(Node* expr, std::ostream& stream) const override {}
    
    virtual std::string toStr() const override {
        return "VARIABLE";
    }
};

class TMonomial : public TType {
public:
    TMonomial() : TType(5) {}
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

class TPolynomial : public TType {
public:
    TPolynomial(const TType* base) : TType(6), base(base) {}
    virtual ~TPolynomial() override {}

    Node* assign(const std::string& name, Node* val) const override {
        return base->polyAssign(name, val);
    }

    virtual bool eq(const TType* other) const override { 
        if (TType::eq(other)) {
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
        const TType* b = ((TPolynomial*)expr->t)->base;
        if (b->eq(TType::INTEGER))
            stream << ((NIntPolyVal*)expr)->getPoly() << std::endl;
        else if (b->eq(TType::RATIONAL))
            stream << ((NRatPolyVal*)expr)->getPoly() << std::endl;
        else if (b->eq(TType::MODULAR))
            stream << ((NModPolyVal*)expr)->getPoly() << std::endl;
    }
    
    virtual std::string toStr() const override {
        return "POLYNOMIAL{" + base->toStr() + "}";
    }

    const TType* getBase() const { return base; }

private:
    const TType* base;
};

void TType::initialize() {
    TType::NOTHING    = new TNothing();
    TType::INTEGER    = new TInteger();
    TType::RATIONAL   = new TRational();
    TType::MODULAR    = new TModular();
    TType::VARIABLE   = new TVariable();
    TType::MONOMIAL   = new TMonomial();
    TType::POLY_INT   = new TPolynomial(TType::INTEGER);
    TType::POLY_RAT   = new TPolynomial(TType::RATIONAL);
    TType::POLY_MOD   = new TPolynomial(TType::MODULAR);
}

void TType::destroy() {
    delete TType::NOTHING;
    delete TType::INTEGER;
    delete TType::RATIONAL;
    delete TType::MODULAR;
    delete TType::VARIABLE;
    delete TType::MONOMIAL;
    delete TType::POLY_INT;
    delete TType::POLY_RAT;
    delete TType::POLY_MOD;
}