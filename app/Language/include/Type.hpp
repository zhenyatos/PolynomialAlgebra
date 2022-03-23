#pragma once
#include <iostream>

class Node;

class Type {
public:
    static const Type* NOTHING;
    static const Type* INTEGER;
    static const Type* RATIONAL;
    static const Type* MODULAR;
    static const Type* VARIABLE;
    static const Type* MONOMIAL;
    static const Type* POLY_INT;
    static const Type* POLY_RAT;
    static const Type* POLY_MOD;

    static void initialize();
    static void destroy();

    Type(size_t code) : code(code) {}
    virtual ~Type() {}

    virtual Node* assign(const std::string& name, Node* val) const = 0;
    virtual void erase(const std::string& name) const {}

    virtual Node* unmin(Node* arg) const;
    virtual Node* abs(Node* arg) const;
    virtual void print(Node* expr, std::ostream& stream) const = 0;
    
    virtual bool eq(const Type* other) const { return code == other->code; }

    virtual Node* val(Node* arg) const { return arg; }
    virtual Node* var(const std::string& name) const { return nullptr; }

    virtual Node* binop(Node* a, const std::string& op, Node* b) const;

    virtual Node* opInt(Node* a, const std::string& op, Node* b) const;
    virtual Node* opRat(Node* a, const std::string& op, Node* b) const; 
    virtual Node* opMod(Node* a, const std::string& op, Node* b) const;
    virtual Node* opPoly(Node* a, const std::string& op, Node* b) const;

    virtual Node* polyAssign(const std::string& name, Node* val) const;
    virtual Node* polyVar(const std::string& name) const { return nullptr; }
    virtual Node* polyMono(Node* c, Node* m) const { return nullptr; }

    virtual std::string toStr() const = 0;

protected:
    size_t code;
};