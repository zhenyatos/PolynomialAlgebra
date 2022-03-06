#include "Parser.hpp"
#include <iostream>

int main() {
    std::vector<Token> tokens = Parser::parse("var a = -1//3 ;");
    for (Token token : tokens)
        std::cout << token.first.name() << " " << token.second << std::endl;
    std::cout << "\n\n";
    tokens = Parser::parse("print a + b;");
    for (Token token : tokens)
        std::cout << token.first.name() << " " << token.second << std::endl;
    return 0;
}