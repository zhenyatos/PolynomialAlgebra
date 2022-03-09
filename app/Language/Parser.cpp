#include "Parser.hpp"
#include "Interpreter.hpp"
#include <string>

const char* c_red = "\u001b[31m";
const char* c_yellow = "\u001b[33m";
const char* c_white = "\u001b[37m";

class NInt : public NIntVal {
public:
    NInt(Integer value)
        : NIntVal(value)
    {}
    ~NInt() override = default;

    void evaluate() override {
        evaluated = true;
    }
};

class NIntOp : public NIntVal {
public:
    NIntOp(Node* left, const std::string& op, Node* right)
        : left(left), op(op), right(right)
    {}
    ~NIntOp() override = default;

    void evaluate() override {
        if (!left->isEval())
            left->evaluate();
        if (!right->isEval())
            right->evaluate();
        Integer a = ((NIntVal*)left)->getValue();
        Integer b = ((NIntVal*)right)->getValue();
        try {
            if (op == "+")
                value = a + b;
            else if (op == "-")
                value = a - b;
            else if (op == "*")
                value = a * b;
            else if (op == "/")
                value = a.div(b);
        } catch (const std::exception& ex) {
            std::cout << c_red << ex.what() << c_white << std::endl;
        }
        
        evaluated = true;
    }

private:
    Node* left;
    Node* right;
    std::string op;
};

class NPrint : public Node {
public:
    NPrint(Node* value, VarType type)
        : expr(value), type(type)
    {}
    ~NPrint() override = default;

    void evaluate() override {
        if (!expr->isEval())
            expr->evaluate();
        if (type == VarType::INTEGER)
            std::cout << ((NIntVal*)expr)->getValue() << std::endl;
        evaluated = true;
    }

private:
    Node* expr;
    VarType type;
};

class NIntAssign : public NIntVal {
public:
    NIntAssign(const std::string& initializer, Node* value)
        : initializer(initializer), expr(value)
    {}
    ~NIntAssign() override = default;

    void evaluate() override {
        if (!expr->isEval())
            expr->evaluate();
        value = ((NIntVal*)expr)->getValue();
        Interpreter::setValue(initializer, value, VarType::INTEGER);
        evaluated = true;
    }

    std::string getName() const {
        return initializer;
    }

private:
    Node* expr;
    std::string initializer;
};

class NIntValVar : public NIntVal {
public:
    NIntValVar(const std::string& name) : name(name) {}
    ~NIntValVar() override = default;

    void evaluate() override {
        value = Interpreter::getIntValue(name, VarType::INTEGER);
        evaluated = true;
    }

private:
    std::string name;
};

class NIntInitVar : public Node {
public:
    NIntInitVar(Node* assignment) 
        : assignment(assignment) 
    {}
    ~NIntInitVar() override = default;

    void evaluate() override {
        std::string name = ((NIntAssign*)assignment)->getName();
        Interpreter::createVariable(name, VarType::INTEGER);
        assignment->evaluate();
        Integer value = ((NIntVal*)assignment)->getValue();
        evaluated = true;
    }

private:
    Node* assignment;
};

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

    Node* res;
    if (tokens[current].first == TokenName::RESERVED_WORD) {
        if (tokens[current].second == "var") {
            eat(TokenName::RESERVED_WORD);
            nodes.push_back(new NIntInitVar(assign()));
            res = nodes.back();
        }
        else if (tokens[current].second == "print") {
            eat(TokenName::RESERVED_WORD);
            nodes.push_back(new NPrint(expr(), VarType::INTEGER));
            res = nodes.back();
        }
    }
    else if (tokens[current].first == TokenName::IDENTIFIER) {
        res = assign();
    }
    else
        res = expr();

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
        nodes.push_back(new NIntAssign(name, expr()));
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
        
        nodes.push_back(new NIntOp(res, token.second, term()));
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

        nodes.push_back(new NIntOp(res, token.second, factor()));
        res = nodes.back();
    }

    return res;
}

Node* Parser::factor() {
    if (current >= tokens.size())
        throw std::runtime_error("Unexpected end of sentence");
    Token token = tokens[current];
    if (token.first == TokenName::NUMBER) {
        eat(TokenName::NUMBER);
        nodes.push_back(new NInt(std::stoi(token.second)));
        return nodes.back();
    } 
    else if (token.first == TokenName::IDENTIFIER) {
        eat(TokenName::IDENTIFIER);
        nodes.push_back(new NIntValVar(token.second));
        return nodes.back();
    }
    else if (token.first == TokenName::LPAREN) {
        eat(TokenName::LPAREN);
        Node* res = expr();
        eat(TokenName::RPAREN);
        return res;
    } else {
        throw std::runtime_error("Unexpected token " + tokens[current].second);
    }
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