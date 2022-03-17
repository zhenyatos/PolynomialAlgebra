#include "Parser.hpp"
#include "Interpreter.hpp"
#include "functions.hpp"
#include <string>
    
Parser::Parser(const std::vector<Token>& tokens) 
    : current(0), tokens(tokens)
{}

void Parser::eat(const TokenName& tokenName) {
    if (current >= tokens.size()) 
        throw std::runtime_error("Unexpected end of sentence");
    else if (tokens[current].first == tokenName)
        current++;
    else
        throw std::runtime_error("Unexpected token: " + tokens[current].second);
}

Node* Parser::sentence() {
    if (current >= tokens.size())
        return nullptr;

    Node* res = expr();

    if (current < tokens.size() && tokens[current].first == TokenName::END_OF_COMMAND) {
        eat(TokenName::END_OF_COMMAND);
        res->evaluate(); 
        res = sentence();
    }

    return res;
}

Node* Parser::assign() {
    if (current >= tokens.size())
        throw std::runtime_error("Unexpected end of sentence");
    if (tokens[current].first == TokenName::IDENTIFIER) {
        std::string name = tokens[current].second;
        eat(TokenName::IDENTIFIER);
        eat(TokenName::ASSIGNMENT);
        Node* res = expr();
        Type t = res->type;
        if (t == Type::INTEGER)
            nodes.push_back(new NIntAssign(name, res));
        else if (t == Type::RATIONAL)
            nodes.push_back(new NRatAssign(name, res));
        return nodes.back();
    } else {
        throw std::runtime_error("Unexpected token " + tokens[current].second);
    }
    
    return nullptr;
}

Node* Parser::expr() {
    Node* res = term();

    while (tokens[current].first == TokenName::PLUS || 
            tokens[current].first == TokenName::MINUS) {
        Token token = tokens[current];
        if (token.first == TokenName::PLUS)
            eat(TokenName::PLUS);
        else
            eat(TokenName::MINUS);
        
        Type ltype = res->type;
        Node* r = term();
        Type rtype = r->type;
        if (ltype == Type::INTEGER && rtype == Type::INTEGER)
            nodes.push_back(new NIntOp(res, token.second, r));
        else if (ltype == Type::RATIONAL || rtype == Type::RATIONAL)
            nodes.push_back(new NRatOp(res, token.second, r));
        else
            throw std::runtime_error("No method matching " + token.second + "(" + std::string(ltype) + ", " +
                                     std::string(rtype) + ")");
        res = nodes.back(); 
    }

    return res;
}

Node* Parser::term() {
    Node* res = factor();

    while (tokens[current].first == TokenName::MUL ||
            tokens[current].first == TokenName::DIV) {
        Token token = tokens[current];
        if (token.first == TokenName::MUL)
            eat(TokenName::MUL);
        else
            eat(TokenName::DIV);

        Type ltype = res->type;
        Node* r = factor();
        Type rtype = r->type;
        if (ltype == Type::INTEGER && rtype == Type::INTEGER)
            nodes.push_back(new NIntOp(res, token.second, r));
        else if (ltype == Type::RATIONAL || rtype == Type::RATIONAL)
            nodes.push_back(new NRatOp(res, token.second, r));
        else {
            throw std::runtime_error("No method matching " + token.second + "(" + std::string(ltype) + ", " +
                                     std::string(rtype) + ")");
        }
        res = nodes.back();
    }

    return res;
}

Node* Parser::factor() {      
    Node* num = number();

    if (num != nullptr)
        if (current >= tokens.size())
            return num;
    
    Token token = tokens[current];
    if (num != nullptr && token.first == TokenName::FRACBAR) {
        eat(TokenName::FRACBAR);
        Node* num2 = number();
        
        if (num2 == nullptr)
            throw std::runtime_error("Unexpected token");
        nodes.push_back(new NRat(num, num2));
        return nodes.back();
    } 
    else if (num != nullptr) {
        return num;
    }
    else if (token.first == TokenName::RESERVED_WORD) {
        if (token.second == "print") {
            eat(TokenName::RESERVED_WORD);
            eat(TokenName::LPAREN);
            nodes.push_back(new NPrint(expr()));
            eat(TokenName::RPAREN);
            return nodes.back();
        } else if (token.second == "abs") {
            eat(TokenName::RESERVED_WORD);
            eat(TokenName::LPAREN);
            Node* arg = expr();
            Type t = arg->type;
            nodes.push_back(abs(arg));
            eat(TokenName::RPAREN);
            return nodes.back();
        }
    }
    else if (token.first == TokenName::IDENTIFIER) {
        eat(TokenName::IDENTIFIER);
        std::string name = token.second;
        if (current >= tokens.size() || tokens[current].first != TokenName::ASSIGNMENT) {
            auto check = Interpreter::variableExists(name);
            if (!check.first)
                throw std::runtime_error("Reference to the uninitialized variable " + name);
            if (check.second == Type::INTEGER)
                nodes.push_back(new NIntValVar(name));
            else if (check.second == Type::RATIONAL)
                nodes.push_back(new NRatValVar(name));
        } else {
            eat(TokenName::ASSIGNMENT);
            Node* res = expr();
            Type t = res->type;
            if (t == Type::INTEGER)
                nodes.push_back(new NIntAssign(name, res));
            else if (t == Type::RATIONAL)
                nodes.push_back(new NRatAssign(name, res));
        }
        return nodes.back();
    }
    else if (token.first == TokenName::LPAREN) {
        eat(TokenName::LPAREN);
        Node* res = expr();
        eat(TokenName::RPAREN);
        return res;
    } else
        throw std::runtime_error("Unexpected token " + tokens[current].second);
}

Node* Parser::number() {
    if (current >= tokens.size())
        throw std::runtime_error("Unexpected end of sentence");

    Token token = tokens[current];
    if (token.first == TokenName::NUMBER) {
        eat(TokenName::NUMBER);
        nodes.push_back(new NInt(std::stoi(token.second)));
        return nodes.back();
    }

    return nullptr;
}

Node* Parser::AST() {
    Node* res = nullptr;
    try {
        res = sentence();
    } catch (const std::exception& ex) {
        std::cout << c_yellow << ex.what() << c_white << std::endl;
        freeNodes();
        return nullptr;
    }
    return res;
}

void Parser::freeNodes() {
    for (Node* node : nodes)
        delete node;
    nodes.clear();
}

const Type Type::NOTHING = Type(0);
const Type Type::INTEGER = Type(1);
const Type Type::RATIONAL = Type(2);
const Type Type::MODULAR = Type(3);

const char* Type::message[4] = {
    "NOTHING",
    "INTEGER",
    "RATIONAL",
    "MODULAR"
};