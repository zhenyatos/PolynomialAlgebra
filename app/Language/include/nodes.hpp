#pragma once
#include "Integer.hpp"
#include "Rational.hpp"
#include "Modular.hpp"
#include "Polynomial.hpp"
#include "Type.hpp"
#define c_red "\u001b[31m"
#define c_yellow "\u001b[33m"
#define c_white "\u001b[37m"

class Node;

class TType {
public:
    static const TType* NOTHING;
    static const TType* INTEGER;
    static const TType* RATIONAL;
    static const TType* MODULAR;
    static const TType* VARIABLE;

    static void initialize();
    static void destroy();

    TType(size_t code) : code(code) {}
    virtual ~TType() {}

    virtual Node* assign(const std::string& name, Node* val) const = 0;
    virtual Node* unmin(Node* arg) const;
    virtual void print(Node* expr, std::ostream& stream) const = 0;
    
    virtual bool eq(const TType* other) const { return code == other->code; }

    virtual Node* val(Node* arg) const { return arg; }

    virtual Node* binop(Node* a, const std::string& op, Node* b) const;

    virtual Node* opInt(Node* a, const std::string& op, Node* b) const;
    virtual Node* opRat(Node* a, const std::string& op, Node* b) const; 
    virtual Node* opMod(Node* a, const std::string& op, Node* b) const;

    virtual Node* polymono(Node* c, Node* m) const { return nullptr; }

    virtual std::string toStr() const = 0;

protected:
    size_t code;
};

class Node {
public:
    Node(Type type, const TType* t) : type(type), t(t) {}
    virtual ~Node() = default;

    virtual void evaluate() = 0;
    bool isEval() { return evaluated; }

    const Type type;
    const TType* t;

protected:
    bool evaluated = false;
};

class NIntVal : public Node {
public:
    NIntVal() : Node(Type::INTEGER, TType::INTEGER) {}
    virtual ~NIntVal() override = default;

    Integer getValue() const { return value; }

protected:
    Integer value;
};

class NRatVal : public Node {
public:
    NRatVal() : Node(Type::RATIONAL, TType::RATIONAL) {}
    virtual ~NRatVal() override = default;

    Rational getValue() const { return value; }

protected:
    Rational value;
};

class NModVal : public Node {
public:
    NModVal() : Node(Type::MODULAR, TType::MODULAR) {}
    virtual ~NModVal() override = default;

    Modular getValue() const { return value; }

protected:
    Modular value;
};

class NVar : public Node {
public:
    NVar(const std::string& name) : Node(Type::VARIABLE, TType::VARIABLE), name(name) {}
    virtual ~NVar() override = default;

    void evaluate() override { evaluated = true; }

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

class NMod : public NModVal {
public:
    NMod(Node* a, Node* N);
    ~NMod() override = default;

    void evaluate() override;

private:
    Node* a;
    Node* N;
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

class NModOp : public NModVal {
public:
    NModOp(Node* left, const std::string& op, Node* right);
    ~NModOp() override = default;

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

class NModAssign : public NModVal {
public:
    NModAssign(const std::string& initializer, Node* value);
    ~NModAssign() override = default;

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

class NModValVar : public NModVal {
public:
    NModValVar(const std::string& name);
    ~NModValVar() override = default;

    void evaluate() override;

private:
    std::string name;
};