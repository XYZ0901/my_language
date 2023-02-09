//
// Created by 周诣鑫 on 2022/10/17.
//

#ifndef MY_LANGUAGE_LEXER_H
#define MY_LANGUAGE_LEXER_H

#include "token.h"

typedef struct Lexer {
    char *input;
    int position;
    int readPosition;
    char ch;
} Lexer;

Lexer *new_lex(char *input);

void destory_lex(Lexer *lex);

void readChar(Lexer *lex);

Token newToken(tokenCode, char);

Token NextToken(Lexer *lex);

char* readIdentifier(Lexer *lex);

int isLetter(char);

#endif //MY_LANGUAGE_LEXER_H
