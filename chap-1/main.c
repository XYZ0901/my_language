//
// Created by 周诣鑫 on 2022/10/17.
//
#include <stdio.h>
#include "lexer.h"

const char *input = {"let five = 5; let ten = 10; let add = fn(x,y){x+y;}; let result = add(five,ten);"};



int main() {
    Lexer *lex = new_lex(input);
    Token tok = NextToken(lex);
    while (tok.Type != tokenCodes[_EOF]) {
        printf("%s %s\n", tok.Type, tok.Literal);
        tok = NextToken(lex);
    }
    return 0;
}