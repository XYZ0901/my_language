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

class Object;

typedef Object *(BuiltinFunction)(std::vector<Object *>);

static enum obj {
    _INTEGER_OBJ,
    _STRING_OBJ,
    _BOOLEAN_OBJ,
    _NULL_OBJ,
    _RETURN_VALUE_OBJ,
    _ERROR_OBJ,
    _FUNCTION_OBJ,
    _BUILTIN,
    _ARRAY_OBJ,
    _HASH_OBJ,
};

static std::map<obj, std::string> ObjectTypes{
        {_INTEGER_OBJ,      "INTEGER"},
        {_STRING_OBJ,       "STRING"},
        {_BOOLEAN_OBJ,      "BOOLEAN"},
        {_NULL_OBJ,         "NULL"},
        {_RETURN_VALUE_OBJ, "RETURN_VALUE"},
        {_ERROR_OBJ,        "ERROR"},
        {_FUNCTION_OBJ,     "FUNCTION"},
        {_BUILTIN,          "BUILTIN"},
        {_ARRAY_OBJ,        "ARRAY"},
        {_HASH_OBJ,         "HASH"},
};

class HashKey {
public:
    std::string String() {
        return Type + std::to_string(Value);
    }

public:
    ObjectType Type;
    unsigned long Value{};
};

class Hashable {
public:
    virtual ~Hashable() = default;

    virtual HashKey HashKey() = 0;
};

class Object {
public:
    virtual ~Object() = default;

    virtual ObjectType Type() = 0;

    virtual std::string Inspect() = 0;
};

class Integer : public Object, public Hashable {
public:
    Integer() = default;

    explicit Integer(long l) : Value(l) {};

    std::string Inspect() override;

    ObjectType Type() override { return ObjectTypes[_INTEGER_OBJ]; }

    class HashKey HashKey() override {
        class HashKey h;
        h.Type = Type();
        std::hash<long> hash_int;
        h.Value = hash_int(Value);
        return h;
    }

public:
    long Value{};
};

class String : public Object, public Hashable {
public:
    String() = default;

    explicit String(std::string s) : Value(std::move(s)) {}

    std::string Inspect() override { return Value; };

    ObjectType Type() override { return ObjectTypes[_STRING_OBJ]; }

    class HashKey HashKey() override {
        class HashKey h;
        h.Type = Type();
        std::hash<std::string> hash_int;
        h.Value = hash_int(Value);
        return h;
    }

public:
    std::string Value;
};

class Bool : public Object, public Hashable {
public:
    Bool() = default;

    explicit Bool(bool b) : Value(b) {};

    std::string Inspect() override;

    ObjectType Type() override { return ObjectTypes[_BOOLEAN_OBJ]; }

    class HashKey HashKey() override {
        class HashKey h;
        h.Type = Type();
        std::hash<bool> hash_bool;
        h.Value = hash_bool(Value);
        return h;
    }

public:
    bool Value{};
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

    explicit environment(environment *e) : outer(e) {}

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

class Builtin : public Object {
public:
    explicit Builtin(BuiltinFunction &bf) : fn(bf) {}

    std::string Inspect() override { return "builtin function"; };

    ObjectType Type() override { return ObjectTypes[_BUILTIN]; }

public:
    BuiltinFunction &fn;
};

class Array : public Object {
public:
    Array() = default;

    explicit Array(std::vector<Object *> &v) : Elements(v) {}

    std::string Inspect() override;

    ObjectType Type() override { return ObjectTypes[_ARRAY_OBJ]; }

public:
    std::vector<Object *> Elements;
};

class HashPair {
public:
    Object *Key;
    Object *Value;
};

class Hash : public Object {
public:
    std::string Inspect() override;

    ObjectType Type() override { return ObjectTypes[_HASH_OBJ]; }

public:
    std::map<std::string, HashPair *> Pairs;
};


#endif //MY_LANGUAGE_OBJECT_H
