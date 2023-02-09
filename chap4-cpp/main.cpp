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

    Start(std::cout,std::cin);

//    test();

//    testEval();

    return 0;
}

void test() {
    std::string input = "[1,2,3,4]";


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
    std::string input = R"(let bob = {"name": "Bob", "age": 99}; bob["name"];)";

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
