//
// Created by 周诣鑫 on 2022/10/18.
//

#ifndef MY_LANGUAGE_PARSER_H
#define MY_LANGUAGE_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <sstream>
#include <map>

class parser;

typedef Expression *(*prefixParseFn)(parser &);

typedef Expression *(*infixParseFn)(parser &, Expression *);

class parser {
    friend Expression *parseIdentifier(parser &);

    friend Expression *parseIntegerLiteral(parser &);

    friend Expression *parseBoolean(parser &);

    friend Expression *parseGroupedExpression(parser &);

    friend Expression *parseIfExpression(parser &);

    friend Expression *parseFunctionLiteral(parser &);

    friend Expression *parsePrefixExpression(parser &);

    friend Expression *parseInfixExpression(parser &, Expression *);

    friend Expression *parseCallExpression(parser &p, Expression *left);

private:
    void nextToken();

    Statement *parseStatement();

    LetStatement *parseLetStatement();

    ReturnStatement *parseReturnStatement();

    ExpressionStatement *parseExpressionStatement();

    BlockStatement *parseBlockStatement();

    std::vector<Identifier *> parseFunctionParameters();

    std::vector<Expression *> parseCallArguments();

    Expression *parseExpression(const int &);

    bool curTokenIs(const TokenType &t) const;

    bool peekTokenIs(const TokenType &t) const;

    bool exceptPeek(const TokenType &t);

    void peekError(const TokenType &);

    void noPrefixParseFnError(const TokenType &t);

    void registerPrefix(const TokenType &t, prefixParseFn f) {
        prefixParseFns[t] = f;
    }

    void registerInfix(const TokenType &t, infixParseFn f) {
        infixParseFns[t] = f;
    }

    int peekPrecedence() const;

    int curPrecedence() const;

public:
    explicit parser(Lexer *);

    Program *ParseProgram();

    std::vector<std::string> Errors();

private:
    Lexer *lex;
    std::vector<std::string> errors;

    Token curToken;
    Token peekToken;

    std::map<TokenType, prefixParseFn> prefixParseFns;
    std::map<TokenType, infixParseFn> infixParseFns;
};

static enum Level {
    LOWEST,
    EQUAL, // == !=
    LESSGREATER, // > < >= <=
    SUM, // +
    PRODUCT, // *
    PREFIX, // -X or !X
    CALL, // function(X)
};

static std::map<TokenType, Level> precedence = {
        {tokenCodes[_EQ], EQUAL},
        {tokenCodes[_NEQ], EQUAL},
        {tokenCodes[_LT], LESSGREATER},
        {tokenCodes[_GT], LESSGREATER},
        {tokenCodes[_LE], LESSGREATER},
        {tokenCodes[_GE], LESSGREATER},
        {tokenCodes[_PLUS], SUM},
        {tokenCodes[_MINUS], SUM},
        {tokenCodes[_SLASH], PRODUCT},
        {tokenCodes[_ASTERISK], PRODUCT},
        {tokenCodes[_LPAREN], CALL}
};

#endif //MY_LANGUAGE_PARSER_H
