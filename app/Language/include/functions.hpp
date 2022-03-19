#pragma once
#include "nodes.hpp"

Node* abs(Node* x);

Node* unmin(Node* x);

Node* gcd(Node* a, Node* b);

Node* binop(Node* l, Node* r, const std::string& op);