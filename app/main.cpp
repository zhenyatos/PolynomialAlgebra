#include "Lexer.hpp"
#include "Parser.hpp"
#include <iostream>

int main() {
    TType::initialize();
    std::vector<Token> tokens;
    std::string input;
    std::getline(std::cin, input);
    while(input != "") {
        tokens = Lexer::parse(input);
        Node* head = nullptr;
        Parser p(tokens);
        head = p.AST();
        if (head != nullptr) {
            try {
                head->evaluate();
            } catch(const std::exception& ex) {
                std::cout << c_red << ex.what() << c_white << std::endl;
            }
            p.freeNodes();
        }
        std::getline(std::cin, input);
    }
    TType::destroy();
    return 0;
}