#include "Lexer.hpp"
#include "Parser.hpp"
#include <iostream>

int main() {
    Type::initialize();
    std::vector<Token> tokens;
    std::string input;
    std::cout << ">>> ";
    std::getline(std::cin, input);
    while(input != "") {
        try {
            tokens = Lexer::parse(input);
        } catch(const std::exception& ex) {
            std::cout << "[LEXER]: " << ex.what() << std::endl;
            std::getline(std::cin, input);
            continue;
        }
        Node* head = nullptr;
        Parser p(tokens);
        head = p.AST();
        if (head != nullptr) {
            try {
                head->evaluate();
            } catch(const std::exception& ex) {
                std::cout << "[INTERPRETER]: " << ex.what() << std::endl;
            } catch(int ex) {
                p.freeNodes();
                Type::destroy();
                return 0;
            }
        }
        p.freeNodes();
        std::cout << "\n>>> ";
        std::getline(std::cin, input);
    }
    Type::destroy();
    return 0;
}