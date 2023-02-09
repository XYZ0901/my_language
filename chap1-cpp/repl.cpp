//
// Created by 周诣鑫 on 2022/10/18.
//

#include "repl.h"
#include "lexer.h"

void Start(std::ostream &os, std::istream &is) {
    std::string s;
    os << PROMPT;
    while (std::getline(is, s)) {
        if (s == "exit") {
            return;
        }
        Lexer lex(s);
        Token tok;
        while ((tok = lex.NextToken()).Type != tokenCodes[_EOF]) {
            os << "Type:" << tok.Type << "  Literal:" << tok.Literal << std::endl;
        }
        os << PROMPT;
    }
}
