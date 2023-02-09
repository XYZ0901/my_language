//
// Created by 周诣鑫 on 2022/10/18.
//

#ifndef MY_LANGUAGE_TOKEN_H
#define MY_LANGUAGE_TOKEN_H

#include <string>
#include <vector>
#include <map>

typedef std::string TokenType;

class Token {
public:
    Token() = default;

    Token(TokenType tp, char ch) : Type(std::move(tp)),
                                   Literal(std::string(1, ch)) {}

    Token(TokenType tp, std::string s) : Type(std::move(tp)),
                                         Literal(std::move(s)) {}

    TokenType Type;
    std::string Literal;
};

static enum tokenCode {
    _ILLEGAL,
    _EOF,

    _IDENT,
    _INT,

    _ASSIGN,
    _PLUS,
    _MINUS,
    _BANG,
    _ASTERISK,
    _SLASH,

    _LT,
    _LE,
    _GT,
    _GE,
    _EQ,
    _NEQ,
    _INC,
    _DEC,

    _COMMA,
    _SEMICOLON,

    _LPAREN,
    _RPAREN,
    _LBRACE,
    _RBRACE,

    _FUNCTION,
    _LET,
    _TRUE,
    _FALSE,
    _IF,
    _ELSE,
    _RETURN,
};

static std::vector<std::string> tokenCodes = {
        "ILLEGAL",
        "EOF",

        "IDENT",
        "INT",

        "=",
        "+",
        "-",
        "!",
        "*",
        "/",

        "<",
        "<=",
        ">",
        ">=",
        "==",
        "!=",
        "++",
        "--",

        ",",
        ";",

        "(",
        ")",
        "{",
        "}",

        "FUNCTION",
        "LET",
        "TRUE",
        "FALSE",
        "IF",
        "ELSE",
        "RETURN"
};

static std::map<std::string, std::string> keywords = {
        {"fn",     tokenCodes[_FUNCTION]},
        {"let",    tokenCodes[_LET]},
        {"true",   tokenCodes[_TRUE]},
        {"false",  tokenCodes[_FALSE]},
        {"if",     tokenCodes[_IF]},
        {"else",   tokenCodes[_ELSE]},
        {"return", tokenCodes[_RETURN]},

};

TokenType LookupIdent(const std::string &);

#endif //MY_LANGUAGE_TOKEN_H
