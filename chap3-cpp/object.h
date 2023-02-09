//
// Created by 周诣鑫 on 2022/10/21.
//

#ifndef MY_LANGUAGE_OBJECT_H
#define MY_LANGUAGE_OBJECT_H

#include <string>
#include <map>
#include <sstream>
#include <vector>

#include "ast.h"

typedef std::string ObjectType;

static enum obj {
    _INTEGER_OBJ,
    _BOOLEAN_OBJ,
    _NULL_OBJ,
    _RETURN_VALUE_OBJ,
    _ERROR_OBJ,
    _FUNCTION_OBJ,
};

static std::map<obj, std::string> ObjectTypes{
        {_INTEGER_OBJ,      "INTEGER"},
        {_BOOLEAN_OBJ,      "BOOLEAN"},
        {_NULL_OBJ,         "NULL"},
        {_RETURN_VALUE_OBJ, "RETURN_VALUE"},
        {_ERROR_OBJ,        "ERROR"},
        {_FUNCTION_OBJ,     "FUNCTION"},
};

class Object {
public:
    virtual ~Object() = default;

    virtual ObjectType Type() = 0;

    virtual std::string Inspect() = 0;
};

class Integer : public Object {
public:
    Integer() = default;

    explicit Integer(long l) : Value(l) {};

    std::string Inspect() override;

    ObjectType Type() override { return ObjectTypes[_INTEGER_OBJ]; }

public:
    long Value{};
};

class Bool : public Object {
public:
    Bool() = default;

    explicit Bool(bool b) : Value(b) {};

    std::string Inspect() override;

    ObjectType Type() override { return ObjectTypes[_BOOLEAN_OBJ]; }

public:
    bool Value;
};

class Null : public Object {
public:
    ~Null() override = default;

    std::string Inspect() override { return "null"; }

    ObjectType Type() override { return ObjectTypes[_NULL_OBJ]; }
};

class ReturnValue : public Object {
public:
    ReturnValue() = default;

    explicit ReturnValue(Object *obj) : Value(obj) {}

    ~ReturnValue() override {
        delete Value;
        Value = nullptr;
    }

    std::string Inspect() override { return Value->Inspect(); }

    ObjectType Type() override { return ObjectTypes[_RETURN_VALUE_OBJ]; }

public:
    Object *Value;
};

class Error : public Object {
public:
    std::string Inspect() override { return "ERROR: " + Message; }

    ObjectType Type() override { return ObjectTypes[_ERROR_OBJ]; }

public:
    std::string Message;
};

class environment {
public:
    environment() = default;
    explicit environment(environment* e):outer(e){}

    Object *Get(const std::string &, bool &);

    Object *Set(const std::string &, Object *);

private:
    std::map<std::string, Object *> store;
    environment *outer;
};

class Function : public Object {
public:
    Function() = default;

    Function(std::vector<Identifier *> &p, BlockStatement *b,
             environment *e) : Parameters(p), Body(b), Env(e) {}

    std::string Inspect() override;

    ObjectType Type() override { return ObjectTypes[_FUNCTION_OBJ]; }

public:
    std::vector<Identifier *> Parameters;
    BlockStatement *Body;
    environment *Env;
};


#endif //MY_LANGUAGE_OBJECT_H
