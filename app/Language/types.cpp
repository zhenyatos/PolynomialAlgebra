#include "nodes.hpp"
#include "polynodes.hpp"
#include "Interpreter.hpp"

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

const TType* TType::NOTHING = nullptr;
const TType* TType::INTEGER = nullptr;
const TType* TType::RATIONAL = nullptr;
const TType* TType::MODULAR = nullptr;

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

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        throw std::runtime_error("No method matching " + op + "(" + a->t->toStr() + ", " +
                                    b->t->toStr() + ")");
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
    
    Node* unmin(Node* arg) const override {
        return new NIntUMin(arg);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        stream << ((NIntVal*)expr)->getValue() << std::endl;
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

    Node* unmin(Node* arg) const override {
        return new NRatUMin(arg);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        stream << ((NRatVal*)expr)->getValue() << std::endl;
    }

    Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opRat(a, op, b);
    }

    virtual Node* opInt(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
    }
    
    virtual Node* opRat(Node* a, const std::string& op, Node* b) const override {
        return new NRatOp(a, op, b);
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

    Node* unmin(Node* arg) const override {
        return new NModUMin(arg);
    }

    void print(Node* expr, std::ostream& stream) const override { 
        stream << ((NModVal*)expr)->getValue() << std::endl;
    }

    virtual Node* binop(Node* a, const std::string& op, Node* b) const override {
        return b->t->opMod(a, op, b);
    }

    virtual Node* opMod(Node* a, const std::string& op, Node* b) const override {
        return new NModOp(a, op, b);
    }

    virtual std::string toStr() const override {
        return "MODULAR";
    }
};

void TType::initialize() {
    TType::NOTHING = new TNothing();
    TType::INTEGER = new TInteger();
    TType::RATIONAL = new TRational();
    TType::MODULAR = new TModular();
}

void TType::destroy() {
    delete TType::NOTHING;
    delete TType::INTEGER;
    delete TType::RATIONAL;
    delete TType::MODULAR;
}