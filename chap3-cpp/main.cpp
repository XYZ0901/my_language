//
// Created by 周诣鑫 on 2022/10/18.
//

#include <iostream>
#include <string>
#include <typeinfo>

#include "lexer.h"
#include "repl.h"
#include "parser.h"
#include "ast.h"
#include "evaluator.h"

void test();

void testEval();

int main(int argc, char **argv) {

//    Start(std::cout,std::cin);

//    test();

    testEval();

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

void testEval(){
    std::string input = "let counter = fn(x) {"
                        "if(x > 100){"
                        "return true;"
                        "}else{"
                        "let foobar = 9999;"
                        "counter(x+1);"
                        "} "
                        "};"
                        "counter(0);";

    Lexer l(input);
    parser p(&l);

    auto program = p.ParseProgram();

    auto env = new environment();
    auto e = Eval(program,env);

    if (e != nullptr){
        std::cout << e->Inspect();
        std::cout << "\n";
    }
    delete env;
}
