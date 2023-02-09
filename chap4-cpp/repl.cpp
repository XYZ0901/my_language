//
// Created by 周诣鑫 on 2022/10/18.
//

#include "repl.h"
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"

void printParserErrors(std::ostream &os,const std::vector<std::string>& errors){
    os << "Woops! We ran into some fucking business here!\n";
    os << " parser errors:\n";

    for (auto &e: errors) {
        os <<"\t"<< e << "\n";
    }
}

void Start(std::ostream &os, std::istream &is) {
    std::string s;
    auto env = new environment;
    os << PROMPT;
    while (std::getline(is, s)) {
        if (s == "exit") {
            return;
        }
        Lexer lex(s);

        parser p(&lex);

        auto program = p.ParseProgram();
        if (!p.Errors().empty()){
            printParserErrors(os,p.Errors());
            os << PROMPT;
            continue;
        }
        auto e = Eval(program,env);
        if (e != nullptr){
            os << e->Inspect();
            os << "\n";
        }
        os << PROMPT;
    }
    delete env;
}
