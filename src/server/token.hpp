#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>

enum class Token {
    X,
    O,
};

inline Token TOKEN = Token::X;

inline std::string token_as_str(Token t) {
    static std::unordered_map<Token, std::string> const table = {
        {Token::O, "O"},
        {Token::X, "X"},
    };

    if (auto search = table.find(t); search != table.end()) {
        return search->second;
    } else {
        return "";
    }
}

#endif
