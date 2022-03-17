#include "Lexer.hpp"
#include <stdexcept>

const std::set<std::string> reservedWords = {
        "abs", "print", "X"
    };

std::vector<Token> Lexer::parse(std::string line) {
    std::vector<Token> result;

    while(line.size() != 0) {
        // white spaces are ignored
        if (line[0] == ' ') {
            line = std::move(line.substr(1));
        } 
        // alphabetic (reserved words and identifiers)
        else if (isalpha(line[0]) > 0) {
            int i = 1;
            while (i < line.size() && isalpha(line[i]) > 0)
                 i++;
            std::string value = line.substr(0, i);
            if (reservedWords.find(value) != reservedWords.end())
                result.push_back({TokenName::RESERVED_WORD, value});
            else 
                result.push_back({TokenName::IDENTIFIER, value});
            line = std::move(line.substr(i));
        } 
        // numeric (number)
        else if (isdigit(line[0]) > 0) {
            int i = 1;
            while (i < line.size() && isdigit(line[i]) > 0)
                i++;
            std::string value = line.substr(0, i);
            result.push_back({TokenName::NUMBER, value});
            line = std::move(line.substr(i));
        }
        // operator / or separator //
        else if (line[0] == '/') {
            if (line.size() != 1 && line[1] == '/') {
                result.push_back({TokenName::FRACBAR, std::string("//")});
                line = std::move(line.substr(2));
            } else {
                result.push_back({TokenName::DIV, line.substr(0, 1)});
                line = std::move(line.substr(1));
            }
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
                result.push_back({TokenName::MINUS, line.substr(0, 1)});
                line = std::move(line.substr(1));
            }
        }
        // single symbol stuff
        else {
            // square brackets
            if (line[0] == '[') 
                result.push_back({TokenName::LSQUARE, line.substr(0, 1)});
            else if (line[0] == ']')
                result.push_back({TokenName::RSQUARE, line.substr(0, 1)});
            // comma and semicolon (end of command)
            else if (line[0] == ',')
                result.push_back({TokenName::COMMA, line.substr(0, 1)});
            else if (line[0] == ';')
                result.push_back({TokenName::END_OF_COMMAND, line.substr(0, 1)});
            // parenthesis
            else if (line[0] == '(') 
                result.push_back({TokenName::LPAREN, line.substr(0, 1)});
            else if (line[0] == ')') 
                result.push_back({TokenName::RPAREN, line.substr(0, 1)});
            // operators + and *
            else if (line[0] == '+')
                result.push_back({TokenName::PLUS, line.substr(0, 1)});
            else if (line[0] == '*')
                result.push_back({TokenName::MUL, line.substr(0, 1)});
            else if (line[0] == '=')
                result.push_back({TokenName::ASSIGNMENT, line.substr(0, 1)});
            // bad
            else
                throw std::invalid_argument("Unexpected character " + line.substr(0, 1));
            // reduce the line
            line = std::move(line.substr(1));
        }
    }

    return result;
}

const char* TokenName::names[15] = {
    "RESERVED_WORD", 
    "PLUS", 
    "MINUS", 
    "MUL", 
    "DIV", 
    "ASSIGNMENT",
    "NUMBER", 
    "IDENTIFIER", 
    "LSQUARE", "RSQUARE", 
    "LPAREN", "RPAREN", 
    "FRACBAR",
    "COMMA", 
    "END_OF_COMMAND"
};

const TokenName TokenName::RESERVED_WORD  = TokenName(0);
const TokenName TokenName::PLUS           = TokenName(1);
const TokenName TokenName::MINUS          = TokenName(2);
const TokenName TokenName::MUL            = TokenName(3);
const TokenName TokenName::DIV            = TokenName(4);
const TokenName TokenName::ASSIGNMENT     = TokenName(5);
const TokenName TokenName::NUMBER         = TokenName(6); 
const TokenName TokenName::IDENTIFIER     = TokenName(7);
const TokenName TokenName::LSQUARE        = TokenName(8);
const TokenName TokenName::RSQUARE        = TokenName(9);
const TokenName TokenName::LPAREN         = TokenName(10); 
const TokenName TokenName::RPAREN         = TokenName(11);
const TokenName TokenName::FRACBAR        = TokenName(12); 
const TokenName TokenName::COMMA          = TokenName(13);
const TokenName TokenName::END_OF_COMMAND = TokenName(14);