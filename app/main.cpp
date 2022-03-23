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
                std::cout << ex.what() << std::endl;
            }
        }
        p.freeNodes();
        for(int i = 0; i < 1000; i++)
            ;
        std::getline(std::cin, input);
    }
    TType::destroy();
    return 0;
}