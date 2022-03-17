#include "Parser.hpp"
#include "Interpreter.hpp"
#include <string>

const char* c_red = "\u001b[31m";
const char* c_yellow = "\u001b[33m";
const char* c_white = "\u001b[37m";

class NInt : public NIntVal {
public:
    NInt(Integer val) { value = val; }
    ~NInt() override = default;

    void evaluate() override {
        evaluated = true;
    }
};

class NRat : public NRatVal {
public:
    NRat(Node* p, Node* q) : p(p), q(q) {}
    ~NRat() override = default;

    void evaluate() override {
        if (p->type != Type::INTEGER)
            throw "BAD";
        if (q->type != Type::INTEGER)
            throw "BAD";
        if (!p->isEval())
            p->evaluate();
        if (!q->isEval())
            q->evaluate();
        value = Rational(((NIntVal*)p)->getValue(), 
                            ((NIntVal*)q)->getValue());
        evaluated = true;
    }

private:
    Node* p;
    Node* q;
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

class NRatOp : public NRatVal {
public:
    NRatOp(Node* left, const std::string& op, Node* right)
        : left(left), op(op), right(right)
    {}
    ~NRatOp() override = default;

    void evaluate() override {
        if (!left->isEval())
            left->evaluate();
        if (!right->isEval())
            right->evaluate();
        Rational a;
        Rational b;
        if (left->type == Type::INTEGER)
            a = Rational(((NIntVal*)left)->getValue());
        else
            a = ((NRatVal*)left)->getValue();
        if (right->type == Type::INTEGER)
            b = Rational(((NIntVal*)right)->getValue());
        else
            b = ((NRatVal*)right)->getValue();
        try {
            if (op == "+")
                value = a + b;
            else if (op == "-")
                value = a - b;
            else if (op == "*")
                value = a * b;
            else if (op == "/")
                value = a / b;
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
    NPrint(Node* value)
        : Node(Type::NOTHING), expr(value)
    {}
    ~NPrint() override = default;

    void evaluate() override {
        if (!expr->isEval())
            expr->evaluate();
        Type type = expr->type;
        if (type == Type::INTEGER)
            std::cout << ((NIntVal*)expr)->getValue() << std::endl;
        else if (type == Type::RATIONAL)
            std::cout << ((NRatVal*)expr)->getValue() << std::endl;
        evaluated = true;
    }

private:
    Node* expr;
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
        try {
            Interpreter::setIntValue(initializer, value);
        } catch (const std::exception& ex) {
            std::cout << c_red << ex.what() << c_white << std::endl;
        }
        evaluated = true;
    }

    std::string getName() const {
        return initializer;
    }

private:
    Node* expr;
    std::string initializer;
};

class NRatAssign : public NRatVal {
public:
    NRatAssign(const std::string& initializer, Node* value)
        : initializer(initializer), expr(value)
    {}
    ~NRatAssign() override = default;

    void evaluate() override {
        if (!expr->isEval())
            expr->evaluate();
        value = ((NRatVal*)expr)->getValue();
        try {
            Interpreter::setRatValue(initializer, value);
        } catch (const std::exception& ex) {
            std::cout << c_red << ex.what() << c_white << std::endl;
        }
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
        try {
            value = Interpreter::getIntValue(name);
        } catch(const std::exception& ex) {
            std::cout << c_red << ex.what() << c_white << std::endl;
        }
        evaluated = true;
    }

private:
    std::string name;
};

class NRatValVar : public NRatVal {
public:
    NRatValVar(const std::string& name) : name(name) {}
    ~NRatValVar() override = default;

    void evaluate() override {
        try {
            value = Interpreter::getRatValue(name);
        } catch(const std::exception& ex) {
            std::cout << c_red << ex.what() << c_white << std::endl;
        }
        evaluated = true;
    }

private:
    std::string name;
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
        if (tokens[current].second == "print") {
            eat(TokenName::RESERVED_WORD);
            eat(TokenName::LPAREN);
            nodes.push_back(new NPrint(expr()));
            eat(TokenName::RPAREN);
            res = nodes.back();
        }
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
        if (ltype == Type::RATIONAL || rtype == Type::RATIONAL)
            nodes.push_back(new NRatOp(res, token.second, r));
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
