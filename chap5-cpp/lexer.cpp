//
// Created by 周诣鑫 on 2022/10/18.
//

#include "lexer.h"

Lexer::Lexer(std::string s) : input(std::move(s)),
                              position(0), readPosition(0), ch('\0') {
    readChar();
}

void Lexer::readChar() {
    if (readPosition >= input.size()) {
        ch = 0;
    } else {
        ch = input[readPosition];
    }
    position = readPosition;
    readPosition++;
}

Token &Lexer::NextToken() {
    Token tok;

    skipWhitespace();

    switch (ch) {
        case '=':
            tok = makeTwoCharToken('=', _EQ, _ASSIGN);
            break;
        case '+':
            tok = makeTwoCharToken('+', _INC, _PLUS);
            break;
        case '-':
            tok = makeTwoCharToken('-', _DEC, _MINUS);
            break;
        case '!':
            tok = makeTwoCharToken('=', _NEQ, _BANG);
            break;
        case '*':
            tok = Token(tokenCodes[_ASTERISK], ch);
            break;
        case '/':
            tok = Token(tokenCodes[_SLASH], ch);
            break;
        case '<':
            tok = makeTwoCharToken('=', _LE, _LT);
            break;
        case '>':
            tok = makeTwoCharToken('=', _GE, _GT);
            break;
        case ';':
            tok = Token(tokenCodes[_SEMICOLON], ch);
            break;
        case ':':
            tok = Token(tokenCodes[_COLON], ch);
            break;
        case '(':
            tok = Token(tokenCodes[_LPAREN], ch);
            break;
        case ')':
            tok = Token(tokenCodes[_RPAREN], ch);
            break;
        case ',':
            tok = Token(tokenCodes[_COMMA], ch);
            break;
        case '{':
            tok = Token(tokenCodes[_LBRACE], ch);
            break;
        case '}':
            tok = Token(tokenCodes[_RBRACE], ch);
            break;
        case '[':
            tok = Token(tokenCodes[_LBRACKET], ch);
            break;
        case ']':
            tok = Token(tokenCodes[_RBRACKET], ch);
            break;
        case '"':
            tok.Type = tokenCodes[_STRING];
            tok.Literal = readString();
            break;
        case 0:
            tok.Literal = "";
            tok.Type = tokenCodes[_EOF];
            break;
        default:
            if (isLetter(ch)) {
                tok.Literal = readIdentifier();
                tok.Type = LookupIdent(tok.Literal);
                return std::ref(tok);
            } else if (isDigit(ch)) {
                tok.Type = tokenCodes[_INT];
                tok.Literal = readNumber();
                return std::ref(tok);
            }
            tok = Token(tokenCodes[_ILLEGAL], ch);
    }
    readChar();
    return std::ref(tok);
}

std::string &Lexer::readIdentifier() {
    int pos = position;
    while (isLetter(ch)) {
        readChar();
    }
    std::string s = input.substr(pos, position - pos);
    return std::ref(s);
}

void Lexer::skipWhitespace() {
    for (; ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';) {
        readChar();
    }
}

std::string &Lexer::readNumber() {
    int pos = position;
    while (isnumber(ch)) {
        readChar();
    }
    std::string s = input.substr(pos, position - pos);
    return std::ref(s);
}

std::string &Lexer::readString() {
    int pos = position + 1;
    for (;;) {
        readChar();
        if (ch == '"' || ch == 0) {
            break;
        }
    }
    std::string s = input.substr(pos, position - pos);
    return std::ref(s);
}

char Lexer::peekChar() {
    if (readPosition >= input.size()) { return 0; }
    return input[readPosition];
}

Token &Lexer::makeTwoCharToken(char v, tokenCode t1, tokenCode t2) {
    Token tok;
    if (peekChar() == v) {
        char c = ch;
        readChar();
        std::string s = std::string(1, c) + ch;
        tok = Token(tokenCodes[t1], s);
    } else {
        tok = Token(tokenCodes[t2], ch);
    }
    return std::ref(tok);
}

bool isLetter(char ch) {
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}

bool isDigit(char ch) {
    return '0' <= ch && ch <= '9';
}