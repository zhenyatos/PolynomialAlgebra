#pragma once
#include <set>
#include <string>
#include <vector>

class TokenName {
public:
    static const TokenName RESERVED_WORD, PLUS, MINUS, MUL, DIV, ASSIGNMENT,
                            NUMBER, IDENTIFIER, LSQUARE, RSQUARE, LPAREN, RPAREN, FRACBAR,
                            COMMA, END_OF_COMMAND;

    bool operator==(const TokenName& other) { return code == other.code; }
    bool operator!=(const TokenName& other) { return code != other.code; }

    const char* name() const { return names[code]; }

private:
    static const char* names[15];
    size_t code;

    TokenName(size_t code) 
        : code(code)
    {};
};

using Token = std::pair<TokenName, std::string>;

class Lexer {
public:
    Lexer() = delete;

    static std::vector<Token> parse(std::string line);
};