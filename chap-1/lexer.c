//
// Created by 周诣鑫 on 2022/10/17.
//

#include <stdlib.h>
#include "lexer.h"
#include <string.h>

Lexer *new_lex(char *input) {
    Lexer *lex = calloc(1, sizeof(Lexer));
    lex->input = input;
    readChar(lex);
    return lex;
}

void destory_lex(Lexer *lex) {
    free(lex);
}

void readChar(Lexer *lex) {
    if (lex->readPosition >= sizeof(lex->input) / sizeof(lex->input[0])) {
        lex->ch = 0;
    } else {
        lex->ch = lex->input[lex->readPosition];
    }
    lex->position = lex->readPosition;
    lex->readPosition++;
}

Token NextToken(Lexer *lex) {
    Token tok;

    switch (lex->ch) {
        case '=':
            tok = newToken(ASSIGN, lex->ch);
            break;
        case ';':
            tok = newToken(SEMICOLON, lex->ch);
            break;
        case '(':
            tok = newToken(LPAREN, lex->ch);
            break;
        case ')':
            tok = newToken(RPAREN, lex->ch);
            break;
        case '{':
            tok = newToken(LBRACE, lex->ch);
            break;
        case '}':
            tok = newToken(RBRACE, lex->ch);
            break;
        case ',':
            tok = newToken(COMMA, lex->ch);
            break;
        case '+':
            tok = newToken(PLUS, lex->ch);
            break;
        case 0:
            tok.Literal = "";
            tok.Type = tokenCodes[_EOF];
            break;
        default:
            if (isLetter(lex->ch)) {
                tok.Literal = readIdentifier(lex);
                return tok;
            } else {
                tok = newToken(ILLEGAL, lex->ch);
            }
    }
    readChar(lex);
    return tok;
}

char* readIdentifier(Lexer *lex){
    int position = lex->position;
    while (isLetter(lex->ch)){
        readChar(lex);
    }
    char re[lex->position - position +1];
    strncpy(re,lex->input,lex->position - position);
    re[lex->position - position] = '\0';
    return re;
}

int isLetter(char ch){
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}

Token newToken(tokenCode tc, char ch) {
    char literal[] = {ch, 0};
    return (Token) {tokenCodes[tc], literal};
}

