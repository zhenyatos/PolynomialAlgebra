#pragma once
#include "Integer.hpp"
#include "Rational.hpp"
#include "Type.hpp"
#define c_red "\u001b[31m"
#define c_yellow "\u001b[33m"
#define c_white "\u001b[37m"

class Node {
public:
    Node(Type type) : type(type) {}
    virtual ~Node() = default;

    virtual void evaluate() = 0;
    bool isEval() { return evaluated; }

    const Type type;

protected:
    bool evaluated = false;
};

class NIntVal : public Node {
public:
    NIntVal() : Node(Type::INTEGER) {}
    virtual ~NIntVal() override = default;

    Integer getValue() const { return value; }

protected:
    Integer value;
};

class NRatVal : public Node {
public:
    NRatVal() : Node(Type::RATIONAL) {}
    virtual ~NRatVal() override = default;

    Rational getValue() const { return value; }

protected:
    Rational value;
};

class NVar : public Node {
public:
    NVar(const std::string& name) : Node(Type::VARIABLE), name(name) {}
    virtual ~NVar() override = default;

    void evaluate() override {}

    Node* value();
    std::string getName() { return name; }

private:
    std::string name;
};

class NInt : public NIntVal {
public:
    NInt(Integer val);
    ~NInt() override = default;

    void evaluate() override;
};

class NRat : public NRatVal {
public:
    NRat(Node* p, Node* q);
    ~NRat() override = default;

    void evaluate() override;

private:
    Node* p;
    Node* q;
};

class NIntOp : public NIntVal {
public:
    NIntOp(Node* left, const std::string& op, Node* right);
    ~NIntOp() override = default;

    void evaluate() override;

private:
    Node* left;
    Node* right;
    std::string op;
};

class NRatOp : public NRatVal {
public:
    NRatOp(Node* left, const std::string& op, Node* right);
    ~NRatOp() override = default;

    void evaluate() override;

private:
    Node* left;
    Node* right;
    std::string op;
};

class NPrint : public Node {
public:
    NPrint(Node* value);
    ~NPrint() override = default;

    void evaluate() override;

private:
    Node* expr;
};

class NIntAssign : public NIntVal {
public:
    NIntAssign(const std::string& initializer, Node* value);
    ~NIntAssign() override = default;

    void evaluate() override;

private:
    Node* expr;
    std::string initializer;
};

class NRatAssign : public NRatVal {
public:
    NRatAssign(const std::string& initializer, Node* value);
    ~NRatAssign() override = default;

    void evaluate() override;

private:
    Node* expr;
    std::string initializer;
};

class NIntValVar : public NIntVal {
public:
    NIntValVar(const std::string& name);
    ~NIntValVar() override = default;

    void evaluate() override;

private:
    std::string name;
};

class NRatValVar : public NRatVal {
public:
    NRatValVar(const std::string& name);
    ~NRatValVar() override = default;

    void evaluate() override;

private:
    std::string name;
};