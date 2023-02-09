//
// Created by 周诣鑫 on 2022/10/21.
//

#ifndef MY_LANGUAGE_EVALUATOR_H
#define MY_LANGUAGE_EVALUATOR_H

#include "object.h"
#include "ast.h"

static Null NIL = Null();
static Bool TRUE = Bool(true);
static Bool FALSE = Bool(false);

Object *Eval(Node *, environment *);

Error *newError(const char *format, ...);

static Object *lenF(std::vector<Object *> args) {
    if (args.size() != 1) {
        return newError("wrong number of arguments. got=%d, want=1",
                        args.size());
    }
    auto s = dynamic_cast<String *>(args[0]);
    if (s != nullptr) {
        return new Integer(long(s->Value.size()));
    }
    auto arr = dynamic_cast<Array *>(args[0]);
    if (arr != nullptr) {
        return new Integer(long(arr->Elements.size()));
    }
    return newError("argument to `len` not supported, got %s",
                    (args[0])->Type().c_str());
}

static Object *firstF(std::vector<Object *> args) {
    if (args.size() != 1) {
        return newError("wrong number of arguments. got=%d, want=1",
                        args.size());
    }
    if (args[0]->Type() != ObjectTypes[_ARRAY_OBJ]) {
        return newError("argument to `first` must be ARRAY, got %s",
                        (args[0])->Type().c_str());
    }
    auto arr = dynamic_cast<Array *>(args[0]);
    if (arr != nullptr && !arr->Elements.empty()) {
        return arr->Elements[0];
    }
    return &NIL;
}

static Object *lastF(std::vector<Object *> args) {
    if (args.size() != 1) {
        return newError("wrong number of arguments. got=%d, want=1",
                        args.size());
    }
    if (args[0]->Type() != ObjectTypes[_ARRAY_OBJ]) {
        return newError("argument to `last` must be ARRAY, got %s",
                        (args[0])->Type().c_str());
    }
    auto arr = dynamic_cast<Array *>(args[0]);
    if (arr != nullptr && !arr->Elements.empty()) {
        return arr->Elements[arr->Elements.size() - 1];
    }
    return &NIL;
}

static Object *restF(std::vector<Object *> args) {
    if (args.size() != 1) {
        return newError("wrong number of arguments. got=%d, want=1",
                        args.size());
    }
    if (args[0]->Type() != ObjectTypes[_ARRAY_OBJ]) {
        return newError("argument to `push` must be ARRAY, got %s",
                        (args[0])->Type().c_str());
    }
    auto arr = dynamic_cast<Array *>(args[0]);
    std::vector<Object *> els;
    if (!arr->Elements.empty()) {
        els.insert(els.begin(), arr->Elements.begin(), arr->Elements.end());
        els.erase(els.begin());
    }
    return new Array(els);
}

static Object *pushF(std::vector<Object *> args) {
    if (args.size() != 2) {
        return newError("wrong number of arguments. got=%d, want=1",
                        args.size());
    }
    if (args[0]->Type() != ObjectTypes[_ARRAY_OBJ]) {
        return newError("argument to `push` must be ARRAY, got %s",
                        (args[0])->Type().c_str());
    }
    auto arr = dynamic_cast<Array *>(args[0]);
    std::vector<Object *> els;
    els.insert(els.begin(), arr->Elements.begin(), arr->Elements.end());
    els.push_back(args[1]);
    return new Array(els);
}

static Object *putsF(std::vector<Object *> args) {
    for (auto &arg: args) {
        std::cout << arg->Inspect() << std::endl;
    }
    return &NIL;
}

static std::map<std::string, Builtin> builtins{
        {"len",   Builtin(lenF)},
        {"first", Builtin(firstF)},
        {"last",  Builtin(lastF)},
        {"push",  Builtin(pushF)},
        {"rest",  Builtin(restF)},
        {"puts",  Builtin(putsF)},
};

class evaluator {

};


#endif //MY_LANGUAGE_EVALUATOR_H
