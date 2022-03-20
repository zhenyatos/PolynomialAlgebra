#pragma once
#include "nodes.hpp"
#include "Integer.hpp"
#include "Rational.hpp"
#include "Type.hpp"

class NMonom : public Node {
public:
    NMonom() : Node(Type::MONOMIAL), deg(1) {}
    virtual ~NMonom() override = default;

    void evaluate() override { evaluated = true; }

    Integer getDeg() const { return deg; }
    
protected:
    Integer deg;
};

class NPowMonom : public NMonom {
public:
    NPowMonom(Node* N);
    ~NPowMonom() override = default;

    void evaluate() override;

private:
    Node* N;
};

class NPolyVal : public Node {
public:
    NPolyVal(Type base) : Node(Type::POLYNOMIAL), base(base) {};
    virtual ~NPolyVal() override = default;

    Type getBase() const { return base; }

protected:
    Type base;
};

class NIntPolyVal : public NPolyVal {
public: 
    NIntPolyVal() : NPolyVal(Type::INTEGER) {}
    virtual ~NIntPolyVal() override = default;

    Polynomial<Integer> getPoly() const { return poly; }

protected:
    Polynomial<Integer> poly;
};

class NIntPolyMono : public NIntPolyVal {
public: 
    NIntPolyMono(Node* c, Node* m);
    ~NIntPolyMono() override = default;

    void evaluate() override;

private:
    Node* c;
    Node* m;
};

class NIntPolyOp : public NIntPolyVal {
public:
    NIntPolyOp(Node* left, const std::string& op, Node* right);
    ~NIntPolyOp() override = default;

    void evaluate() override;

private:
    Node* left;
    Node* right;
    std::string op;
};

class NIntPolyAssign : public NIntPolyVal {
public:
    NIntPolyAssign(const std::string& initializer, Node* value);
    ~NIntPolyAssign() override = default;

    void evaluate() override;

private:
    Node* expr;
    std::string initializer;
};

class NIntPolyValVar : public NIntPolyVal {
public:
    NIntPolyValVar(const std::string& name);
    ~NIntPolyValVar() override = default;

    void evaluate() override;

private:
    std::string name;
};