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

    if (current < tokens.size()) {
        if (tokens[current].first == TokenName::END_OF_COMMAND) {
            eat(TokenName::END_OF_COMMAND);
            res->evaluate(); 
            freeNodes();
            res = sentence();
        }
        else
            throw std::runtime_error("Unexpected token " + tokens[current].second + " after end of expression");
    } else {
        nodes.push_back(new NPrint(res));
        return nodes.back();
    }

    return res;
}

Node* Parser::statement() {
    if (end())
        throw std::runtime_error("Unexpected end of line");
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
            else if (t == Type::MODULAR)
                nodes.push_back(new NModAssign(name, res));
            else
                throw std::runtime_error("Can't assign " + std::string(t));
        } else {
            nodes.push_back(var->value());
        }
        return nodes.back();
    }

    return l;
}

Node* Parser::expr() {
    if (end())
        throw std::runtime_error("Unexpected end of line");
    Node* l = term();

    while (tokens[current].first == TokenName::PLUS || 
            tokens[current].first == TokenName::MINUS) {
        Token token = tokens[current];
        if (token.first == TokenName::PLUS)
            eat(TokenName::PLUS);
        else
            eat(TokenName::MINUS);
        
        l = varval(l);
        Node* r = varval(term());
        
        nodes.push_back(binop(l, r, token.second));
        l = nodes.back(); 
    }

    return l;
}

Node* Parser::term() {
    if (end())
        throw std::runtime_error("Unexpected end of line");
    Node* l = subterm();

    while (tokens[current].first == TokenName::MUL ||
            tokens[current].first == TokenName::DIV) {
        Token token = tokens[current];
        if (token.first == TokenName::MUL)
            eat(TokenName::MUL);
        else
            eat(TokenName::DIV);
        
        l = varval(l);
        Node* r = varval(subterm());

        nodes.push_back(binop(l, r, token.second));
        l = nodes.back();
    }

    return l;
}

Node* Parser::subterm() {
    if (end())
        throw std::runtime_error("Unexpected end of line");
    Node* l = factor();

    while (tokens[current].first == TokenName::FRACBAR) {
        eat(TokenName::FRACBAR);

        l = varval(l);
        Node* r = varval(factor());
        Type ltype = l->type;
        Type rtype = r->type;
        if (ltype == Type::INTEGER && rtype == Type::INTEGER)
            nodes.push_back(new NRat(l, r));
        else if (ltype == Type::RATIONAL || rtype == Type::RATIONAL)
            nodes.push_back(new NRatOp(l, "/", r));
        else
            throw std::runtime_error("No method matching //(" + std::string(ltype) + ", " +
                                     std::string(rtype) + ")");
        
        l = nodes.back();
    }

    return l;
}

Node* Parser::factor() {
    if (end())
        throw std::runtime_error("Unexpected end of line");
    if (tokens[current].first == TokenName::MINUS) {
        eat(TokenName::MINUS);
        Node* p = varval(prime());
        nodes.push_back(unmin(p));
        return nodes.back();
    }

    Node* p = prime();
    if (current >= tokens.size())
        return p;
    Type ltype, rtype;
    if (tokens[current].first == TokenName::POWER) {
        eat(TokenName::POWER);
        p = varval(p);
        Node* n = varval(factor());
        ltype = p->type;
        rtype = n->type;
        if (ltype == Type::MONOMIAL && rtype == Type::INTEGER) {
            nodes.push_back(new NPowMonom(n));
            return nodes.back();
        } 
        else
            throw std::runtime_error("No method matching ^(" + std::string(ltype) + ", " + 
                                        std::string(rtype) + ")");
    }
    else if (tokens[current].first == TokenName::DOT) {
        eat(TokenName::DOT);
        p = varval(p);
        Node* m = varval(factor());
        ltype = p->type;
        rtype = m->type;
        if (rtype == Type::MONOMIAL) {
            if (ltype == Type::INTEGER) {
                nodes.push_back(new NIntPolyMono(p, m));
                return nodes.back();
            }
        }
        else
            throw std::runtime_error("No method matching .(" + std::string(ltype) + ", " +
                                        std::string(rtype) + ")");
    }

    return p;
}

Node* Parser::prime() {     
    if (end())
        throw std::runtime_error("Unexpected end of line");
    Token token = tokens[current];

    if (token.first == TokenName::NUMBER) {
        eat(TokenName::NUMBER);
        nodes.push_back(new NInt(std::stoi(token.second)));
        return nodes.back();
    }
    else if (token.first == TokenName::RESERVED_WORD) {
        eat(TokenName::RESERVED_WORD);
        if (token.second == "X")
            nodes.push_back(new NMonom());
        else {
            eat(TokenName::LPAREN);
            if (token.second == "print")
                nodes.push_back(new NPrint(statement()));
            else if (token.second == "abs")
                nodes.push_back(abs(statement()));
            else if (token.second == "gcd") {
                Node* l = statement();
                eat(TokenName::COMMA);
                Node* r = statement();
                nodes.push_back(gcd(l, r));
            }
            eat(TokenName::RPAREN);
        }
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
    } 
    else if (token.first == TokenName::LSQUARE) {
        eat(TokenName::LSQUARE);
        Node* a = statement();
        eat(TokenName::COMMA);
        Node* N = statement();
        eat(TokenName::RSQUARE);
        nodes.push_back(new NMod(a, N));
        return nodes.back();
    }
    else
        throw std::runtime_error("Unexpected token " + tokens[current].second);
}

Node* Parser::varval(Node* arg) {
    if (arg->type == Type::VARIABLE) {
        nodes.push_back(((NVar*)arg)->value());
        return nodes.back();
    }
    return arg;
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
const Type Type::MONOMIAL = Type(5);
const Type Type::POLYNOMIAL = Type(6);

const char* Type::message[7] = {
    "NOTHING",
    "INTEGER",
    "RATIONAL",
    "MODULAR",
    "VARIABLE",
    "MONOMIAL",
    "POLYNOMIAL"
};