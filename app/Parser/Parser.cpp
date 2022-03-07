#include "Parser.hpp"
#include <stdexcept>

const std::set<std::string> reservedWords = {
        "var", "print", "X"
    };

std::vector<Token> Parser::parse(std::string line) {
    std::vector<Token> result;

    while(line.size() != 0) {
        // white spaces are ignored
        if (line[0] == ' ') {
            line = std::move(line.substr(1));
        } else
        // alphabetic (reserved words and identifiers)
        if (isalpha(line[0]) > 0) {
            int i = 1;
            while (i < line.size() && isalpha(line[i]) > 0)
                 i++;
            std::string value = line.substr(0, i);
            if (reservedWords.find(value) != reservedWords.end())
                result.push_back({TokenName::RESERVED_WORD, value});
            else 
                result.push_back({TokenName::IDENTIFIER, value});
            line = std::move(line.substr(i));
        } else
        // numeric (number)
        if (isdigit(line[0]) > 0) {
            int i = 1;
            while (i < line.size() && isdigit(line[i]) > 0)
                i++;
            std::string value = line.substr(0, i);
            result.push_back({TokenName::NUMBER, value});
            line = std::move(line.substr(i));
        } else
        // separators, except //
        if (line[0] == '[' || line[0] == ']' || 
            line[0] == ',' || line[0] == '.') {
            result.push_back({TokenName::SEPARATOR, line.substr(0, 1)});
            line = std::move(line.substr(1));
        } else
        // operator / or separator //
        if (line[0] == '/') {
            if (line.size() != 1 && line[1] == '/') {
                result.push_back({TokenName::SEPARATOR, std::string("//")});
                line = std::move(line.substr(2));
            } else {
                result.push_back({TokenName::OPERATOR, line.substr(0, 1)});
                line = std::move(line.substr(1));
            }
        }
        // operators, excluding / and -
        else if (line[0] == '+' || line[0] == '*' || line[0] == '=') {
            result.push_back({TokenName::OPERATOR, line.substr(0, 1)});
            line = std::move(line.substr(1));
        } 
        // operator - or number
        else if (line[0] == '-') {
            if (line.size() != 1 && isdigit(line[1]) > 0) {
                int i = 1;
                while (i < line.size() && isdigit(line[i]) > 0)
                    i++;
                std::string value = line.substr(0, i);
                result.push_back({TokenName::NUMBER, value});
                line = std::move(line.substr(i));
            } else {
                result.push_back({TokenName::OPERATOR, line.substr(0, 1)});
                line = std::move(line.substr(1));
            }
        }
        // ; (end of command)
        else if (line[0] == ';') {
            result.push_back({TokenName::END_OF_COMMAND, line.substr(0, 1)});
            line = std::move(line.substr(1));
        }
        // bad
        else 
            throw std::invalid_argument("Unexpected character " + line.substr(0, 1));
    }

    return result;
}

const char* TokenName::names[6] = {
    "RESERVED_WORD",
    "OPERATOR",
    "NUMBER",
    "IDENTIFIER",
    "SEPARATOR",
    "END_OF_COMMAND"
};

const TokenName TokenName::RESERVED_WORD = TokenName(0);
const TokenName TokenName::OPERATOR = TokenName(1);
const TokenName TokenName::NUMBER = TokenName(2);
const TokenName TokenName::IDENTIFIER = TokenName(3);
const TokenName TokenName::SEPARATOR = TokenName(4);
const TokenName TokenName::END_OF_COMMAND = TokenName(5);