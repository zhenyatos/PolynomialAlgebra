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

    Node* res = statement();

    if (current < tokens.size() && tokens[current].first == TokenName::END_OF_COMMAND) {
        eat(TokenName::END_OF_COMMAND);
        res->evaluate(); 
        freeNodes();
        res = sentence();
    } else {
        nodes.push_back(new NPrint(res));
        return nodes.back();
    }

    return res;
}

Node* Parser::statement() {
    Node* l = expr();

    if (l->type == Type::VARIABLE) {
        NVar* var = (NVar*)l;
        std::string name = var->getName();
        if (current < tokens.size() && tokens[current].first == TokenName::ASSIGNMENT) {
            eat(TokenName::ASSIGNMENT);
            Node* res = statement();
            Type t = res->type;
            if (t == Type::INTEGER)
                nodes.push_back(new NIntAssign(name, res));
            else if (t == Type::RATIONAL)
                nodes.push_back(new NRatAssign(name, res));
            else
                throw std::runtime_error("Can't assign " + std::string(t));
        } else {
            nodes.push_back(var->value());
        }
        return nodes.back();
    }

    return l;
}

// expr : term +- term (+- term ...)
Node* Parser::expr() {
    Node* l = term();

    while (tokens[current].first == TokenName::PLUS || 
            tokens[current].first == TokenName::MINUS) {
        Token token = tokens[current];
        if (token.first == TokenName::PLUS)
            eat(TokenName::PLUS);
        else
            eat(TokenName::MINUS);
        
        if (l->type == Type::VARIABLE) {
            nodes.push_back(((NVar*)l)->value());
            l = nodes.back();
        }
        Type ltype = l->type;
        Node* r = term();
        if (r->type == Type::VARIABLE) {
            nodes.push_back(((NVar*)r)->value());
            r = nodes.back();
        }
        Type rtype = r->type;
        if (ltype == Type::INTEGER && rtype == Type::INTEGER)
            nodes.push_back(new NIntOp(l, token.second, r));
        else if (ltype == Type::RATIONAL || rtype == Type::RATIONAL)
            nodes.push_back(new NRatOp(l, token.second, r));
        else
            throw std::runtime_error("No method matching " + token.second + "(" + std::string(ltype) + ", " +
                                     std::string(rtype) + ")");
        l = nodes.back(); 
    }

    return l;
}

//  term : factor */ factor (*/ factor ...)
Node* Parser::term() {
    Node* l = factor();

    while (tokens[current].first == TokenName::MUL ||
            tokens[current].first == TokenName::DIV) {
        Token token = tokens[current];
        if (token.first == TokenName::MUL)
            eat(TokenName::MUL);
        else
            eat(TokenName::DIV);
        
        if (l->type == Type::VARIABLE) {
            nodes.push_back(((NVar*)l)->value());
            l = nodes.back();
        }
        Type ltype = l->type;
        Node* r = factor();
        if (r->type == Type::VARIABLE) {
            nodes.push_back(((NVar*)r)->value());
            r = nodes.back();
        }
        Type rtype = r->type;
        if (ltype == Type::INTEGER && rtype == Type::INTEGER)
            nodes.push_back(new NIntOp(l, token.second, r));
        else if (ltype == Type::RATIONAL || rtype == Type::RATIONAL)
            nodes.push_back(new NRatOp(l, token.second, r));
        else
            throw std::runtime_error("No method matching " + token.second + "(" + std::string(ltype) + ", " +
                                     std::string(rtype) + ")");
        
        l = nodes.back();
    }

    return l;
}

// factor  : number | number // number | print(expr) | abs(expr) | IDENTIFIER | LPAREN expr RPAREN
Node* Parser::factor() {      
    Node* num = number();

    if (num != nullptr)
        if (current >= tokens.size())
            return num;
    
    Token token = tokens[current];
    if (num != nullptr) {
        if (token.first == TokenName::FRACBAR) {
            eat(TokenName::FRACBAR);
            Node* num2 = expr();
            
            nodes.push_back(new NRat(num, num2));
            return nodes.back();
        } 
        else
            return num;
    }
    else if (token.first == TokenName::RESERVED_WORD) {
        eat(TokenName::RESERVED_WORD);
        eat(TokenName::LPAREN);
        if (token.second == "print")
            nodes.push_back(new NPrint(statement()));
        else if (token.second == "abs")
            nodes.push_back(abs(statement()));
        eat(TokenName::RPAREN);
        return nodes.back();
    }
    else if (token.first == TokenName::IDENTIFIER) {
        eat(TokenName::IDENTIFIER);
        std::string name = token.second;
        nodes.push_back(new NVar(name));
        return nodes.back();
    }
    else if (token.first == TokenName::LPAREN) {
        eat(TokenName::LPAREN);
        Node* res = statement();
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
const Type Type::VARIABLE = Type(4);

const char* Type::message[5] = {
    "NOTHING",
    "INTEGER",
    "RATIONAL",
    "MODULAR",
    "VARIABLE"
};