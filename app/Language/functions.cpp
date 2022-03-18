#include "functions.hpp"

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
    if (t == Type::INTEGER) {
        return new NIntUMin(x);
    } else if (t == Type::RATIONAL) {
        return new NRatUMin(x);
    } else
        throw std::runtime_error("No function matching -(" + std::string(t) + ")");
}