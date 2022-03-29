#include "Parser.hpp"
#include "Interpreter.hpp"
#include "functions.hpp"
#include "polynodes.hpp"
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
            try { 
                res->evaluate(); 
            } catch(const std::exception& ex) {
                std::cout << "[INTERPRETER]: " << ex.what() << std::endl;
            } catch(int ex) {
                nodes.push_back(new NExit());
                return nodes.back();
            }
            freeNodes();
            res = AST();
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

    if (l->t->eq(Type::VARIABLE)) {
        NVar* var = (NVar*)l;
        std::string name = var->getName();
        if (current < tokens.size() && tokens[current].first == TokenName::ASSIGNMENT) {
            eat(TokenName::ASSIGNMENT);
            Node* res = statement();
            nodes.push_back(assign(name, res));
            return nodes.back();
        } else
            return varval(l);
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
    Node* l = concat();

    while (tokens[current].first == TokenName::MUL ||
            tokens[current].first == TokenName::DIV) {
        Token token = tokens[current];
        if (token.first == TokenName::MUL)
            eat(TokenName::MUL);
        else
            eat(TokenName::DIV);

        l = varval(l);
        Node* r = varval(concat());
        nodes.push_back(binop(l, r, token.second));
        
        l = nodes.back();
    }

    return l;
}

Node* Parser::concat() {
    if (end())
        throw std::runtime_error("Unexpected end of line");
    Node* l = subterm();

    if (current >= tokens.size())
        return l;

    if (tokens[current].first == TokenName::DOT) {
        eat(TokenName::DOT);
        l = varval(l);
        Node* r = varval(factor());
        if (r->t->eq(Type::MONOMIAL)) {
            nodes.push_back(polymono(l, r));
            return nodes.back();
        }
        else
            throw std::runtime_error("No method matching .(" + l->t->toStr() + ", " +
                                        r->t->toStr() + ")");
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
        if (l->t->eq(Type::INTEGER) && r->t->eq(Type::INTEGER))
            nodes.push_back(new NRat(l, r));
        else if (l->t->eq(Type::RATIONAL) || r->t->eq(Type::RATIONAL))
            nodes.push_back(new NRatOp(l, "/", r));
        else
            throw std::runtime_error("No method matching //(" + l->t->toStr() + ", " +
                                     r->t->toStr() + ")");
        
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
    if (tokens[current].first == TokenName::POWER) {
        eat(TokenName::POWER);
        p = varval(p);
        Node* n = varval(factor());
        if (p->t->eq(Type::MONOMIAL) && n->t->eq(Type::INTEGER)) {
            nodes.push_back(new NPowMonom(n));
            return nodes.back();
        } 
        else
            throw std::runtime_error("No method matching ^(" + p->t->toStr() + ", " + 
                                        n->t->toStr() + ")");
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
            else if (token.second == "eval") {
                Node* p = statement();
                eat(TokenName::COMMA);
                Node* x = statement();
                nodes.push_back(peval(p, x));
            }
            else if (token.second == "exit") 
                nodes.push_back(new NExit());
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
    Node* res = arg->t->val(arg);
    if (res != arg)
        nodes.push_back(res);
    return res;
}

Node* Parser::AST() {
    Node* res = nullptr;
    try {
        res = sentence();
    } catch (const std::exception& ex) {
        std::cout << "[PARSER]: " << ex.what() << std::endl;
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
