#include "Parser.hpp"
#include <iostream>

int main() {
    std::vector<Token> tokens = Parser::parse("var a = -1//3 ;");
    for (Token token : tokens)
        std::cout << token.first.name() << " " << token.second << std::endl;
    std::cout << "\n\n";
    try {
    tokens = Parser::parse("print a + b;>");
    } catch(std::exception& err) {
        std::cout << err.what();
        return 0;
    }
    for (Token token : tokens)
        std::cout << token.first.name() << " " << token.second << std::endl;
    return 0;
}