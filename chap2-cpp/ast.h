//
// Created by 周诣鑫 on 2022/10/18.
//

#ifndef MY_LANGUAGE_AST_H
#define MY_LANGUAGE_AST_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "token.h"

class Node {
public:
    virtual ~Node() = default;

    virtual std::string TokenLiteral() = 0;

    virtual std::string String() = 0;
};

class Statement : public Node {
public:
    virtual ~Statement() = default;

    virtual void statementNode() = 0;
};

class Expression : public Node {
public:
    virtual ~Expression() = default;

    virtual void expressionNode() = 0;
};

class Program : public Node {
public:
    ~Program() {
        auto iter = Statements.begin();
        while (iter != Statements.end()) {
            delete *iter++;
        }
        Statements.clear();
    }

    std::vector<Statement *> Statements;

    std::string TokenLiteral() override;

    std::string String() override;
};

class Identifier : public Expression {
public:
    void expressionNode() override {};

    std::string TokenLiteral() override { return tok.Literal; };

    std::string String() override;

public:
    Token tok;
    std::string value;
};

class LetStatement : public Statement {
public:
    ~LetStatement() {
        delete Name;
        Name = nullptr;
        delete Value;
        Value = nullptr;
    }

    void statementNode() override {};

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    Identifier *Name;
    Expression *Value;
};

class ReturnStatement : public Statement {
public:
    ~ReturnStatement() {
        delete ReturnValue;
        ReturnValue = nullptr;
    }

    void statementNode() override {};

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    Expression *ReturnValue;
};

class ExpressionStatement : public Statement {
public:
    ~ExpressionStatement() {
        delete Expression;
        Expression = nullptr;
    }

    void statementNode() override {};

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    Expression *Expression;
};

class IntegerLiteral : public Expression {
public:
    void expressionNode() override {};

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override { return tok.Literal; }

public:
    Token tok;
    long Value;
};

class PrefixExpression : public Expression {
public:
    ~PrefixExpression() {
        delete Right;
        Right = nullptr;
    }

    void expressionNode() override {};

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    std::string Operator;
    Expression *Right;
};

class InfixExpression : public Expression {
public:
    ~InfixExpression() {
        delete Left;
        Left = nullptr;
        delete Right;
        Right = nullptr;
    }

    void expressionNode() override {};

    std::string TokenLiteral() override { return tok.Literal; };

    std::string String() override;

public:
    Token tok;
    Expression *Left;
    std::string Operator;
    Expression *Right;
};

class Boolean : public Expression {
public:
    void expressionNode() override {}

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override { return tok.Literal; }

public:
    Token tok;
    bool Value;
};

class BlockStatement : public Statement {
public:
    ~BlockStatement() {
        auto iter = Statements.begin();
        while (iter != Statements.end()) {
            delete *iter++;
        }
        Statements.clear();
    }

    void statementNode() override {}

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    std::vector<Statement *> Statements;
};

class IfExpression : public Expression {
public:
    ~IfExpression() {
        delete Condition;
        Condition = nullptr;
        delete Consequence;
        Consequence = nullptr;
        delete Alternative;
        Alternative = nullptr;
    }

    void expressionNode() override {}

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    Expression *Condition;
    BlockStatement *Consequence;
    BlockStatement *Alternative;
};

class FunctionLiteral : public Expression {
public:
    ~FunctionLiteral() {
        auto iter = Parameters.begin();
        while (iter != Parameters.end()) {
            delete *iter++;
        }
        Parameters.clear();
        delete Body;
        Body = nullptr;
    }

    void expressionNode() override {}

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    std::vector<Identifier *> Parameters;
    BlockStatement *Body;
};

class CallExpression : public Expression {
public:
    ~CallExpression() {
        delete Function;
        Function = nullptr;
        auto b = Argument.begin();
        while (b != Argument.end()) {
            delete *b++;
        }
        Argument.clear();
    }

    void expressionNode() override {}

    std::string TokenLiteral() override { return tok.Literal; }

    std::string String() override;

public:
    Token tok;
    Expression *Function;
    std::vector<Expression *> Argument;
};

#endif //MY_LANGUAGE_AST_H
