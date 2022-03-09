#include "Lexer.hpp"
#include "Parser.hpp"
#include <iostream>

int main() {
    std::vector<Token> tokens;
    std::string input;
    std::getline(std::cin, input);
    while(input != "") {
        tokens = Lexer::parse(input);
        Node* head = nullptr;
        Parser p(tokens);
        head = p.AST();
        if (head != nullptr) {
            head->evaluate();
            p.freeNodes();
        }
        std::getline(std::cin, input);
    }
    return 0;
}