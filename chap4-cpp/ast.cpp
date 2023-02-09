//
// Created by 周诣鑫 on 2022/10/18.
//

#include "ast.h"

std::string Program::TokenLiteral() {
    if (!Statements.empty()) {
        return Statements[0]->TokenLiteral();
    }
    return "";
}

std::string Program::String() {
    std::ostringstream buf;

    for (auto &i: Statements) {
        buf << i->String() << std::endl;
    }

    return buf.str();
}

std::string LetStatement::String() {
    std::ostringstream buf;

    buf << TokenLiteral() << " " << Name->String() << " = ";
    if (Value != nullptr) {
        buf << Value->String();
    }
    buf << ";";
    return buf.str();
}

std::string ReturnStatement::String() {
    std::ostringstream buf;

    buf << TokenLiteral() << " ";
    if (ReturnValue != nullptr) {
        buf << ReturnValue->String();
    }
    buf << ";";
    return buf.str();
}

std::string ExpressionStatement::String() {
    if (Expression != nullptr) {
        return Expression->String();
    }
    return "";
}

std::string Identifier::String() {
    return value;
}

std::string PrefixExpression::String() {
    std::ostringstream buf;
    buf << "(" << Operator << Right->String() << ")";
    return buf.str();
}

std::string InfixExpression::String() {
    std::ostringstream buf;
    buf << "(" << Left->String() << " "
        << Operator << " " << Right->String() << ")";
    return buf.str();
}

std::string BlockStatement::String() {
    std::ostringstream buf;
    for (auto &s: Statements) {
        buf << s->String();
    }
    return buf.str();
}

std::string IfExpression::String() {
    std::ostringstream buf;
    buf << "if" << Condition->String() << " ";
    buf << Consequence->String();
    if (Alternative != nullptr) {
        buf << "else " << Alternative->String();
    }
    return buf.str();
}

std::string FunctionLiteral::String() {
    std::ostringstream buf;
    buf << "(";
    if (!Parameters.empty()) {
        buf << Parameters[0]->String();
        auto b = ++Parameters.begin();
        while (b != Parameters.end()) {
            buf << ", " << (*b++)->String();
        }
    }
    buf << ") ";
    buf << Body->String();
    return buf.str();
}

std::string CallExpression::String() {
    std::ostringstream buf;
    buf << Function->String();
    buf << "(";
    if (!Argument.empty()) {
        buf << Argument[0]->String();
        auto b = ++Argument.begin();
        while (b != Argument.end()) {
            buf << ", " << (*b++)->String();
        }
    }
    buf << ") ";
    return buf.str();
}

std::string ArrayLiteral::String() {
    std::ostringstream buf;

    buf << "[";
    if (!Elements.empty()) {
        buf << Elements[0]->String();
        auto b = Elements.begin();
        while (++b != Elements.end()) {
            buf << ", " << (*b)->String();
        }
    }
    buf << "]";
    return buf.str();
}

std::string IndexExpression::String() {
    std::ostringstream buf;
    buf << "(" << left->String() << "[" << index->String() << "])";
    return buf.str();
}

std::string HashLiteral::String() {
    std::ostringstream buf;
    buf << "{";
    if (!Pairs.empty()) {
        auto b = Pairs.begin();
        buf << b->first->String() + ":" + b->second->String();
        while (++b != Pairs.end()) {
            buf << ", " << b->first->String() + ":" + b->second->String();
        }
    }
    buf << "}";
    return buf.str();
}
