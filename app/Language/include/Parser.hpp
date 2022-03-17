#pragma once
#include "Lexer.hpp"
#include "Type.hpp"
#include "Integer.hpp"
#include "Rational.hpp"
#include <iostream>

class Parser;

class Node {
public:
    Node(Type type) : type(type) {}
    virtual ~Node() = default;

    virtual void evaluate() = 0;
    
    bool isEval() { return evaluated; }

    friend Parser;

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

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ~Parser() = default;

    Node* AST();

    void freeNodes();

private:
    size_t current;
    std::vector<Token> tokens;

    void eat(const TokenName& tokenName);
    Node* sentence();
    Node* assign();
    Node* expr();
    Node* term();
    Node* factor();
    Node* number();

    std::vector<Node*> nodes;
};