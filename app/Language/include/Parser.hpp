#pragma once
#include "Lexer.hpp"
#include "Integer.hpp"
#include <iostream>

class Parser;

class Node {
public:
    Node() = default;
    virtual ~Node() = default;

    virtual void evaluate() = 0;
    
    bool isEval() { return evaluated; }

    friend Parser;

protected:
    bool evaluated = false;
};

class NIntVal : public Node {
public:
    NIntVal() = default;
    NIntVal(Integer value) : value(value) {}
    ~NIntVal() override = default;

    Integer getValue() const { return value; }

protected:
    Integer value;
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

    std::vector<Node*> nodes;
};