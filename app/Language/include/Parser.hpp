#pragma once
#include "Lexer.hpp"
#include "Type.hpp"
#include "nodes.hpp"
#include <iostream>

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
    Node* statement();
    Node* expr();
    Node* term();
    Node* factor();
    Node* number();

    std::vector<Node*> nodes;
};