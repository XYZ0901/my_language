//
// Created by 周诣鑫 on 2022/10/18.
//

#ifndef MY_LANGUAGE_LEXER_H
#define MY_LANGUAGE_LEXER_H

#include <string>
#include <vector>
#include "token.h"

class Lexer {
public:
    explicit Lexer(std::string s);

    Token &NextToken();

private:
    void readChar();

    std::string &readIdentifier();

    void skipWhitespace();

    std::string &readNumber();

    char peekChar();

    Token& makeTwoCharToken(char,tokenCode,tokenCode);

private:
    std::string input;
    int position, readPosition;
    char ch;
};

bool isLetter(char ch);
bool isDigit(char ch);

#endif //MY_LANGUAGE_LEXER_H
