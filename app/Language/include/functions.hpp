#pragma once
#include "nodes.hpp"

Node* abs(Node* x);

Node* unmin(Node* x);

Node* gcd(Node* a, Node* b);

Node* binop(Node* l, Node* r, const std::string& op);

Node* monomial(Node* l, Node* r);

Node* polyop(Node* l, Node* r, const std::string& op);

Node* peval(Node* p, Node* x);

Node* assign(std::string name, Node* val);

Node* polymono(Node* c, Node* m);