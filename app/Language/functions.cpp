#include "functions.hpp"
#include "polynodes.hpp"

class NIntGCD : public NIntVal {
public:
    NIntGCD(Node* a, Node* b) : a(a), b(b) {}
    ~NIntGCD() override = default;

    void evaluate() override {
        if (!a->isEval())
            a->evaluate();
        if (!b->isEval())
            b->evaluate();
        value = GCD(((NIntVal*)a)->getValue(), ((NIntVal*)b)->getValue());
    }

private:
    Node* a;
    Node* b;
};

Node* abs(Node* x) {
    return x->t->abs(x);
}

Node* unmin(Node* x) {
    return x->t->unmin(x);
}

Node* gcd(Node* a, Node* b) {
    if (!a->t->eq(Type::INTEGER) && !b->t->eq(Type::INTEGER))
        throw std::runtime_error("No function matching gcd(" + a->t->toStr() + ", " +
                                b->t->toStr() + ")");
    return new NIntGCD(a, b);       
}

Node* binop(Node* l, Node* r, const std::string& op) {
    return l->t->binop(l, op, r);
}

Node* peval(Node* p, Node* x) {
    return p->t->getPT()->eval(p, x);
}

Node* assign(std::string name, Node* val) {
    return val->t->assign(name, val);
}

Node* polymono(Node* c, Node* m) {
    return c->t->getPT()->mono(c, m);
}