#pragma once
#include "nodes.hpp"
#include "Integer.hpp"
#include "Rational.hpp"

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

class NIntPolyVal : public Node {
public: 
    NIntPolyVal() : Node(Type::POLY_INT) {}
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

class NRatPolyVal : public Node {
public: 
    NRatPolyVal() : Node(Type::POLY_RAT) {}
    virtual ~NRatPolyVal() override = default;

    Polynomial<Rational> getPoly() const { return poly; }

protected:
    Polynomial<Rational> poly;
};

class NRatPolyMono : public NRatPolyVal {
public: 
    NRatPolyMono(Node* c, Node* m);
    ~NRatPolyMono() override = default;

    void evaluate() override;

private:
    Node* c;
    Node* m;
};

class NRatPolyOp : public NRatPolyVal {
public:
    NRatPolyOp(Node* left, const std::string& op, Node* right);
    ~NRatPolyOp() override = default;

    void evaluate() override;

private:
    Node* left;
    Node* right;
    std::string op;
};

class NRatPolyAssign : public NRatPolyVal {
public:
    NRatPolyAssign(const std::string& initializer, Node* value);
    ~NRatPolyAssign() override = default;

    void evaluate() override;

private:
    Node* expr;
    std::string initializer;
};

class NRatPolyValVar : public NRatPolyVal {
public:
    NRatPolyValVar(const std::string& name);
    ~NRatPolyValVar() override = default;

    void evaluate() override;

private:
    std::string name;
};

class NModPolyVal : public Node {
public: 
    NModPolyVal() : Node(Type::POLY_MOD) {}
    virtual ~NModPolyVal() override = default;

    Polynomial<Modular> getPoly() const { return poly; }

protected:
    Polynomial<Modular> poly;
};

class NModPolyMono : public NModPolyVal {
public: 
    NModPolyMono(Node* c, Node* m);
    ~NModPolyMono() override = default;

    void evaluate() override;

private:
    Node* c;
    Node* m;
};

class NModPolyOp : public NModPolyVal {
public:
    NModPolyOp(Node* left, const std::string& op, Node* right);
    ~NModPolyOp() override = default;

    void evaluate() override;

private:
    Node* left;
    Node* right;
    std::string op;
};

class NModPolyAssign : public NModPolyVal {
public:
    NModPolyAssign(const std::string& initializer, Node* value);
    ~NModPolyAssign() override = default;

    void evaluate() override;

private:
    Node* expr;
    std::string initializer;
};

class NModPolyValVar : public NModPolyVal {
public:
    NModPolyValVar(const std::string& name);
    ~NModPolyValVar() override = default;

    void evaluate() override;

private:
    std::string name;
};

Polynomial<Rational> cast(const Polynomial<Integer>& p);