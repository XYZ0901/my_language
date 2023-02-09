//
// Created by 周诣鑫 on 2022/10/18.
//

#include "parser.h"
#include <string>

Expression *parseIdentifier(parser &p) {
    auto i = new Identifier;
    i->tok = p.curToken;
    i->value = p.curToken.Literal;
    return i;
}

Expression *parseIntegerLiteral(parser &p) {
    auto i = new IntegerLiteral;
    i->tok = p.curToken;
    try {
        i->Value = std::stol(p.curToken.Literal);
    } catch (std::out_of_range &e) {
        std::ostringstream msg;
        msg << "could out parse " << p.curToken.Literal << " as integer";
        p.errors.push_back(msg.str());
        return nullptr;
    }
    return i;
}

Expression *parseArrayLiteral(parser &p) {
    auto arr = new ArrayLiteral;
    arr->tok = p.curToken;

    arr->Elements = p.parseExpressionList(tokenCodes[_RBRACKET]);

    return arr;
}

Expression *parseStringLiteral(parser &p) {
    auto i = new StringLiteral;
    i->tok = p.curToken;
    i->Value = p.curToken.Literal;
    return i;
}

Expression *parseBoolean(parser &p) {
    auto i = new Boolean;
    i->tok = p.curToken;
    i->Value = p.curTokenIs(tokenCodes[_TRUE]);
    return i;
}

Expression *parseGroupedExpression(parser &p) {
    p.nextToken();
    auto exp = p.parseExpression(LOWEST);
    if (!p.exceptPeek(tokenCodes[_RPAREN])) {
        return nullptr;
    }
    return exp;
}

Expression *parseHashLiteral(parser &p){
    auto hash = new HashLiteral;

    while (!p.peekTokenIs(tokenCodes[_RBRACE])) {
        p.nextToken();
        auto key = p.parseExpression(LOWEST);
        if (!p.exceptPeek(tokenCodes[_COLON])){
            return nullptr;
        }
        p.nextToken();
        auto value = p.parseExpression(LOWEST);
        hash->Pairs[key] = value;
        if (!p.peekTokenIs(tokenCodes[_RBRACE]) && !p.exceptPeek(tokenCodes[_COMMA])){
            return nullptr;
        }
    }
    if (!p.exceptPeek(tokenCodes[_RBRACE])){
        return nullptr;
    }

    return hash;
}

Expression *parseIfExpression(parser &p) {
    auto i = new IfExpression;
    if (!p.exceptPeek(tokenCodes[_LPAREN])) {
        return nullptr;
    }
    p.nextToken();
    i->Condition = p.parseExpression(LOWEST);
    if (!p.exceptPeek(tokenCodes[_RPAREN])) {
        return nullptr;
    }
    if (!p.exceptPeek(tokenCodes[_LBRACE])) {
        return nullptr;
    }
    i->Consequence = p.parseBlockStatement();

    if (p.peekTokenIs(tokenCodes[_ELSE])) {
        p.nextToken();
        if (!p.exceptPeek(tokenCodes[_LBRACE])) {
            return nullptr;
        }
        i->Alternative = p.parseBlockStatement();
    }
    return i;
}

Expression *parseFunctionLiteral(parser &p) {
    auto i = new FunctionLiteral;
    i->tok = p.curToken;
    if (!p.exceptPeek(tokenCodes[_LPAREN])) {
        return nullptr;
    }
    i->Parameters = p.parseFunctionParameters();
    if (!p.exceptPeek(tokenCodes[_LBRACE])) {
        return nullptr;
    }
    i->Body = p.parseBlockStatement();
    return i;
}

Expression *parsePrefixExpression(parser &p) {
    auto i = new PrefixExpression;
    i->tok = p.curToken;
    i->Operator = p.curToken.Literal;

    p.nextToken();
    i->Right = p.parseExpression(PREFIX);
    return i;
}

Expression *parseInfixExpression(parser &p, Expression *left) {
    auto i = new InfixExpression;
    i->tok = p.curToken;
    i->Operator = p.curToken.Literal;
    i->Left = left;
    int precede = p.curPrecedence();
    p.nextToken();
    i->Right = p.parseExpression(precede);
    return i;
}

Expression *parseCallExpression(parser &p, Expression *left) {
    auto exp = new CallExpression;
    exp->tok = p.curToken;
    exp->Function = left;
    exp->Argument = p.parseExpressionList(tokenCodes[_RPAREN]);
    return exp;
}

Expression *parseIndexExpression(parser &p, Expression *left) {
    auto exp = new IndexExpression;
    exp->tok = p.curToken;
    exp->left = left;
    p.nextToken();
    exp->index = p.parseExpression(LOWEST);
    if (!p.exceptPeek(tokenCodes[_RBRACKET])) {
        return nullptr;
    }
    return exp;
}

parser::parser(Lexer *l) : lex(l) {
    nextToken();
    nextToken();

    registerPrefix(tokenCodes[_IDENT], &parseIdentifier);
    registerPrefix(tokenCodes[_INT], &parseIntegerLiteral);
    registerPrefix(tokenCodes[_STRING], &parseStringLiteral);
    registerPrefix(tokenCodes[_BANG], &parsePrefixExpression);
    registerPrefix(tokenCodes[_MINUS], &parsePrefixExpression);
    registerPrefix(tokenCodes[_INC], &parsePrefixExpression);
    registerPrefix(tokenCodes[_DEC], &parsePrefixExpression);

    registerPrefix(tokenCodes[_TRUE], &parseBoolean);
    registerPrefix(tokenCodes[_FALSE], &parseBoolean);

    registerPrefix(tokenCodes[_LPAREN], &parseGroupedExpression);
    registerPrefix(tokenCodes[_LBRACE], &parseHashLiteral);
    registerPrefix(tokenCodes[_LBRACKET], &parseArrayLiteral);
    registerPrefix(tokenCodes[_IF], &parseIfExpression);
    registerPrefix(tokenCodes[_FUNCTION], &parseFunctionLiteral);

    registerInfix(tokenCodes[_LPAREN], &parseCallExpression);
    registerInfix(tokenCodes[_LBRACKET], &parseIndexExpression);
    registerInfix(tokenCodes[_PLUS], parseInfixExpression);
    registerInfix(tokenCodes[_MINUS], parseInfixExpression);
    registerInfix(tokenCodes[_SLASH], parseInfixExpression);
    registerInfix(tokenCodes[_ASTERISK], parseInfixExpression);
    registerInfix(tokenCodes[_EQ], parseInfixExpression);
    registerInfix(tokenCodes[_NEQ], parseInfixExpression);
    registerInfix(tokenCodes[_LT], parseInfixExpression);
    registerInfix(tokenCodes[_GT], parseInfixExpression);
    registerInfix(tokenCodes[_LE], parseInfixExpression);
    registerInfix(tokenCodes[_GE], parseInfixExpression);

}

void parser::nextToken() {
    curToken = peekToken;
    peekToken = lex->NextToken();
}

Program *parser::ParseProgram() {
    auto p = new Program;

    while (curToken.Type != tokenCodes[_EOF]) {
        auto stmt = parseStatement();
        if (stmt != nullptr) {
            p->Statements.push_back(stmt);
        }
        nextToken();
    }

    return p;
}

Statement *parser::parseStatement() {
    if (curToken.Type == tokenCodes[_LET]) {
        return parseLetStatement();
    }
    if (curToken.Type == tokenCodes[_RETURN]) {
        return parseReturnStatement();
    }
    return parseExpressionStatement();
}

LetStatement *parser::parseLetStatement() {
    auto stmt = new LetStatement();
    stmt->tok = curToken;

    if (!exceptPeek(tokenCodes[_IDENT])) {
        return nullptr;
    }

    stmt->Name = new Identifier();
    stmt->Name->tok = curToken;
    stmt->Name->value = curToken.Literal;

    if (!exceptPeek(tokenCodes[_ASSIGN])) {
        return nullptr;
    }

    nextToken();
    stmt->Value = parseExpression(LOWEST);

    while (!curTokenIs(tokenCodes[_SEMICOLON]) && !curTokenIs(tokenCodes[_EOF])) {
        nextToken();
    }

    return stmt;
}

ReturnStatement *parser::parseReturnStatement() {
    auto stmt = new ReturnStatement();
    stmt->tok = curToken;

    nextToken();

    stmt->ReturnValue = parseExpression(LOWEST);

    while (!curTokenIs(tokenCodes[_SEMICOLON]) && !curTokenIs(tokenCodes[_EOF])) {
        nextToken();
    }

    return stmt;
}

ExpressionStatement *parser::parseExpressionStatement() {
    auto stmt = new ExpressionStatement();

    stmt->Expression = parseExpression(LOWEST);
    if (peekTokenIs(tokenCodes[_SEMICOLON])) {
        nextToken();
    }

    return stmt;
}

Expression *parser::parseExpression(const int &precede) {
    if (prefixParseFns.find(curToken.Type) == prefixParseFns.end()) {
        noPrefixParseFnError(curToken.Type);
        return nullptr;
    }
    auto prefix = prefixParseFns[curToken.Type];
    auto leftExp = prefix(*this);

    while (!peekTokenIs(tokenCodes[_SEMICOLON]) && precede < peekPrecedence()) {
        if (infixParseFns.find(peekToken.Type) == infixParseFns.end()) {
            return nullptr;
        }
        auto infix = infixParseFns[peekToken.Type];

        nextToken();
        leftExp = infix(*this, leftExp);
    }
    return leftExp;
}

BlockStatement *parser::parseBlockStatement() {
    auto block = new BlockStatement;
    block->tok = curToken;

    nextToken();

    while (!curTokenIs(tokenCodes[_RBRACE]) && !curTokenIs(tokenCodes[_EOF])) {
        auto stmt = parseStatement();
        if (stmt != nullptr) {
            block->Statements.push_back(stmt);
        }
        nextToken();
    }
    return block;
}

std::vector<Identifier *> parser::parseFunctionParameters() {
    std::vector<Identifier *> is;

    if (peekTokenIs(tokenCodes[_RPAREN])) {
        nextToken();
        return is;
    }
    nextToken();

    auto i = new Identifier;
    i->tok = curToken;
    i->value = curToken.Literal;
    is.push_back(i);

    while (peekTokenIs(tokenCodes[_COMMA])) {
        nextToken();
        nextToken();
        auto ident = new Identifier;
        ident->tok = curToken;
        ident->value = curToken.Literal;
        is.push_back(ident);
    }
    if (!exceptPeek(tokenCodes[_RPAREN])) {
        return {};
    }
    return is;
}

std::vector<Expression *> parser::parseCallArguments() {
    std::vector<Expression *> args;

    if (peekTokenIs(tokenCodes[_RPAREN])) {
        nextToken();
        return args;
    }

    nextToken();
    args.push_back(parseExpression(LOWEST));
    while (peekTokenIs(tokenCodes[_COMMA])) {
        nextToken();
        nextToken();
        args.push_back(parseExpression(LOWEST));
    }
    if (!exceptPeek(tokenCodes[_RPAREN])) {
        return {};
    }

    return args;
}

std::vector<Expression *> parser::parseExpressionList(const std::string &end) {
    std::vector<Expression *> list;

    if (peekTokenIs(end)) {
        nextToken();
        return list;
    }

    nextToken();
    list.push_back(parseExpression(LOWEST));
    while (peekTokenIs(tokenCodes[_COMMA])) {
        nextToken();
        nextToken();
        list.push_back(parseExpression(LOWEST));
    }

    if (!exceptPeek(end)) {
        return {};
    }

    return list;
}

bool parser::curTokenIs(const TokenType &t) const {
    return curToken.Type == t;
}

bool parser::peekTokenIs(const TokenType &t) const {
    return peekToken.Type == t;
}

bool parser::exceptPeek(const TokenType &t) {
    if (peekTokenIs(t)) {
        nextToken();
        return true;
    }
    peekError(t);
    return false;
}

std::vector<std::string> parser::Errors() {
    return errors;
}

void parser::peekError(const TokenType &t) {
    std::ostringstream buf;

    buf << "expected next token to be " <<
        t << ", got " << peekToken.Type << " instead";
    std::string msg = buf.str();

    errors.push_back(msg);
}

void parser::noPrefixParseFnError(const TokenType &t) {
    std::ostringstream buf;
    buf << "no prefix parse function for " << t << " found";
    errors.push_back(buf.str());
}

int parser::peekPrecedence() const {
    if (precedence.find(peekToken.Type) != precedence.end()) {
        return precedence[peekToken.Type];
    }
    return LOWEST;
}

int parser::curPrecedence() const {
    if (precedence.find(curToken.Type) != precedence.end()) {
        return precedence[curToken.Type];
    }
    return LOWEST;
}

