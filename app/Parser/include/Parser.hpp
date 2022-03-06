#pragma once
#include <set>
#include <string>
#include <vector>

class TokenName {
public:
    static const TokenName RESERVED_WORD;
    static const TokenName OPERATOR;
    static const TokenName NUMBER;
    static const TokenName IDENTIFIER;
    static const TokenName SEPARATOR;

    bool operator==(const TokenName& other) { return code == other.code; }
    bool operator!=(const TokenName& other) { return code != other.code; }

    const char* name() const { return names[code]; }

private:
    static const char* names[5];
    size_t code;

    TokenName(size_t code) 
        : code(code)
    {};
};

using Token = std::pair<TokenName, std::string>;

class Parser {
public:
    Parser() = delete;

    static std::vector<Token> parse(std::string line);
};