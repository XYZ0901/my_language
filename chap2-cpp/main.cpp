//
// Created by 周诣鑫 on 2022/10/18.
//

#include <iostream>
#include <string>

#include "lexer.h"
#include "repl.h"
#include "parser.h"
#include "ast.h"

void test();

int main(int argc, char **argv) {

    Start(std::cout,std::cin);

//    test();

    return 0;
}

void test() {
    std::string input = "let a = fn(x,y) {"
                        "x+y+5;"
                        "if (1==false) {"
                        "y+1;"
                        "}else{"
                        "y+2;"
                        "}"
                        "return y;}";


    Lexer lex = Lexer(input);

    parser p = parser(&lex);

    Program *program = p.ParseProgram();

    auto errors = p.Errors();

    auto b = errors.begin();
    while (b != errors.end()) {
        std::cout << *b++ << std::endl;
    }

    std::cout << program->String() << std::endl;
}