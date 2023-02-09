//
// Created by 周诣鑫 on 2022/10/17.
//

#ifndef MY_LANGUAGE_TOKEN_H
#define MY_LANGUAGE_TOKEN_H

typedef char *TokenType;

typedef struct Token {
    TokenType Type;
    char *Literal;
} Token;

typedef enum tokenCode {
    ILLEGAL,
    _EOF,

    IDENT,
    INT,

    ASSIGN,
    PLUS,

    COMMA,
    SEMICOLON,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    FUNCTION,
    LET
} tokenCode;

static char *tokenCodes[] = {
        "ILLEGAL",
        "EOF",
        "IDENT",
        "INT",
        "=",
        "+",
        ",",
        ";",
        "(",
        ")",
        "{",
        "}",
        "FUNCTION",
        "LET"};

#endif //MY_LANGUAGE_TOKEN_H
