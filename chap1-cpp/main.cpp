//
// Created by 周诣鑫 on 2022/10/18.
//

#include <iostream>
#include <string>

#include "lexer.h"
#include "repl.h"

void test();

int main(int argc, char **argv) {

    Start(std::cout,std::cin);
    return 0;
}

void test(){
    std::string input = "let five = 5;"
                        "let ten = 10;"
                        "let add = fn(x, y){x + y;};"
                        "let result = add(five, ten);"
                        "!-/*5;"
                        "5 < 10 > 5;"
                        "if (5 < 10) {"
                        "return true;"
                        "}else{"
                        "return false;"
                        "}"
                        "10 == 10;"
                        "10 != 9;"
                        "10 <= 10;"
                        "10 >= 9;";

    Lexer lex = Lexer(input);

    Token tok;

    while ((tok = lex.NextToken()).Type != tokenCodes[_EOF] && tok.Type != tokenCodes[_ILLEGAL]) {
        std::cout << tok.Type << " : " << tok.Literal << std::endl;
    }
}