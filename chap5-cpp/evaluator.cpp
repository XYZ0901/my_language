//
// Created by 周诣鑫 on 2022/10/21.
//

#include "evaluator.h"
#include <string>

Object *evalProgram(std::vector<Statement *> &, environment *);

Object *evalBlockStatements(BlockStatement *, environment *);

Object *evalIfExpression(IfExpression *, environment *);

std::vector<Object *> evalExpressions(std::vector<Expression *> &, environment *);

Object *evalIdentifier(Identifier *, environment *);

Object *evalHashLiteral(HashLiteral *, environment *);

Object *evalPrefixExpression(const std::string &, Object *);

Object *evalBangOperatorExpression(Object *);

Object *evalMinusPrefixOperatorExpression(Object *);

Object *evalIncPrefixOperatorExpression(Object *);

Object *evalDecPrefixOperatorExpression(Object *);

Object *evalInfixExpression(const std::string &, Object *, Object *);

Object *evalIntegerInfixExpression(const std::string &, Object *, Object *);

Object *evalStringInfixExpression(const std::string &op, Object *l, Object *r);

Object *nativeBoolToBooleanObject(bool in);

Object *evalIndexExpression(Object *, Object *);

Object *evalArrayIndexExpression(Object *, Object *);

Object *evalHashIndexExpression(Object *arr, Object *idx);

Object *applyFunction(Object *, std::vector<Object *> &);

environment *extendFunctionEnv(Function *, std::vector<Object *> &);

Object *unwrapReturnValue(Object *);

bool isError(Object *);

Object *Eval(Node *node, environment *env) {

    auto p = dynamic_cast<Program *>(node);
    if (p != nullptr) {
        return evalProgram(p->Statements, env);
    }

    auto e = dynamic_cast<ExpressionStatement *>(node);
    if (e != nullptr) {
        return Eval(e->Expression, env);
    }

    auto le = dynamic_cast<LetStatement *>(node);
    if (le != nullptr) {
        auto val = Eval(le->Value, env);
        if (isError(val)) {
            return val;
        }
        env->Set(le->Name->value, val);
    }

    auto id = dynamic_cast<Identifier *>(node);
    if (id != nullptr) {
        return evalIdentifier(id, env);
    }

    auto ife = dynamic_cast<IfExpression *>(node);
    if (ife != nullptr) {
        return evalIfExpression(ife, env);
    }

    auto bs = dynamic_cast<BlockStatement *>(node);
    if (bs != nullptr) {
        return evalBlockStatements(bs, env);
    }

    auto re = dynamic_cast<ReturnStatement *>(node);
    if (re != nullptr) {
        auto val = Eval(re->ReturnValue, env);
        if (isError(val)) {
            return val;
        }
        return new ReturnValue(val);
    }

    auto fn = dynamic_cast<FunctionLiteral *>(node);
    if (fn != nullptr) {
        auto param = fn->Parameters;
        auto bo = fn->Body;
        return new Function(param, bo, env);
    }

    auto ca = dynamic_cast<CallExpression *>(node);
    if (ca != nullptr) {
        auto fu = Eval(ca->Function, env);
        if (isError(fu)) {
            return fu;
        }
        auto args = evalExpressions(ca->Argument, env);
        if (args.size() == 1 && isError(args[0])) {
            return args[0];
        }
        return applyFunction(fu, args);
    }

    auto pre = dynamic_cast<PrefixExpression *>(node);
    if (pre != nullptr) {
        auto r = Eval(pre->Right, env);
        if (isError(r)) {
            return r;
        }
        return evalPrefixExpression(pre->Operator, r);
    }

    auto inf = dynamic_cast<InfixExpression *>(node);
    if (inf != nullptr) {
        auto l = Eval(inf->Left, env);
        if (isError(l)) {
            return l;
        }
        auto r = Eval(inf->Right, env);
        if (isError(r)) {
            return r;
        }
        return evalInfixExpression(inf->Operator, l, r);
    }

    auto idx = dynamic_cast<IndexExpression *>(node);
    if (idx != nullptr) {
        auto l = Eval(idx->left, env);
        if (isError(l)) {
            return l;
        }
        auto ind = Eval(idx->index, env);
        if (isError(ind)) {
            return ind;
        }
        return evalIndexExpression(l, ind);
    }

    auto i = dynamic_cast<IntegerLiteral *>(node);
    if (i != nullptr) {
        auto o = new Integer;
        o->Value = i->Value;
        return o;
    }

    auto s = dynamic_cast<StringLiteral *>(node);
    if (s != nullptr) {
        return new String(s->Value);
    }

    auto arr = dynamic_cast<ArrayLiteral *>(node);
    if (arr != nullptr) {
        auto els = evalExpressions(arr->Elements, env);
        if (els.size() == 1 && isError(els[0])) {
            return els[0];
        }
        return new Array(els);
    }

    auto mh = dynamic_cast<HashLiteral *>(node);
    if (mh != nullptr) {
        return evalHashLiteral(mh, env);
    }

    auto ai = dynamic_cast<IndexExpression *>(node);
    if (ai != nullptr) {
        auto left = Eval(ai->left, env);
        if (isError(left)) {
            return left;
        }
        auto index = Eval(ai->index, env);
        if (isError(index)) {
            return index;
        }
        return evalIndexExpression(left, index);
    }

    auto b = dynamic_cast<Boolean *>(node);
    if (b != nullptr) {
        return nativeBoolToBooleanObject(b->Value);
    }

    return nullptr;
}

Object *evalProgram(std::vector<Statement *> &stmts, environment *env) {
    Object *res;
    for (auto &s: stmts) {
        res = Eval(s, env);

        auto ro = dynamic_cast<ReturnValue *> (res);
        if (ro != nullptr) {
            return ro->Value;
        }

        auto e = dynamic_cast<Error *>(res);
        if (e != nullptr) {
            return e;
        }
    }
    return res;
}

Object *evalBlockStatements(BlockStatement *stmts, environment *env) {
    Object *res;
    for (auto &s: stmts->Statements) {
        res = Eval(s, env);

        if (res != nullptr) {
            auto rt = res->Type();
            if (rt == ObjectTypes[_RETURN_VALUE_OBJ] ||
                rt == ObjectTypes[_ERROR_OBJ]) {
                return res;
            }
        }
    }
    return res;
}

Object *evalIfExpression(IfExpression *ie, environment *env) {
    // if condition ? Consequence : Alternative;
    auto cond = Eval(ie->Condition, env);
    if (isError(cond)) {
        return cond;
    }
    if (!(cond->Inspect() == "FALSE" || cond->Inspect() == "null")) {
        return Eval(ie->Consequence, env);
    }
    if (ie->Alternative != nullptr) {
        return Eval(ie->Alternative, env);
    }
    return &NIL;
}

std::vector<Object *> evalExpressions(std::vector<Expression *> &exps, environment *env) {
    std::vector<Object *> res;
    for (auto &exp: exps) {
        auto ev = Eval(exp, env);
        if (isError(ev)) {
            return {ev};
        }
        res.push_back(ev);
    }
    return res;
}

Object *evalIdentifier(Identifier *id, environment *env) {
    bool b;
    auto o = env->Get(id->value, b);
    if (b) {
        return o;
    }
    auto fi = builtins.find(id->value);
    if (fi != builtins.end()) {
        return &(fi->second);
    }
    std::string err = "identifier not found: " + id->value;
    return newError(err.c_str());
}

Object *evalPrefixExpression(const std::string &op, Object *obj) {
    if (op == "!") {
        return evalBangOperatorExpression(obj);
    }
    if (op == "-") {
        return evalMinusPrefixOperatorExpression(obj);
    }
    if (op == "--") {
        return evalDecPrefixOperatorExpression(obj);
    }
    if (op == "++") {
        return evalIncPrefixOperatorExpression(obj);
    }
    return newError("unknown operator: %s%s", op.c_str(), obj->Type().c_str());
}

Object *evalBangOperatorExpression(Object *obj) {
    auto n = dynamic_cast<Null *>(obj);
    if (n != nullptr) {
        return &TRUE;
    }

    auto b = dynamic_cast<Bool *>(obj);
    if (b != nullptr) {
        if (b->Value) {
            return &FALSE;
        }
        return &TRUE;
    }

    return &FALSE;
}

Object *evalMinusPrefixOperatorExpression(Object *obj) {
    if (obj->Type() != ObjectTypes[_INTEGER_OBJ]) {
        return newError("unknown operator: -%s", obj->Type().c_str());
    }
    auto i = dynamic_cast<Integer *>(obj);
    if (i != nullptr) {
        auto v = i->Value;
        auto in = new Integer;
        in->Value = -v;
        return in;
    }
    return nullptr;
}

Object *evalIncPrefixOperatorExpression(Object *obj) {
    if (obj->Type() != ObjectTypes[_INTEGER_OBJ]) {
        return newError("unknown operator: ++%s", obj->Type().c_str());
    }
    auto i = dynamic_cast<Integer *>(obj);
    if (i != nullptr) {
        auto v = i->Value;
        auto in = new Integer;
        in->Value = ++v;
        return in;
    }
    return nullptr;
}

Object *evalDecPrefixOperatorExpression(Object *obj) {
    if (obj->Type() != ObjectTypes[_INTEGER_OBJ]) {
        return newError("unknown operator: --%s", obj->Type().c_str());
    }
    auto i = dynamic_cast<Integer *>(obj);
    if (i != nullptr) {
        auto v = i->Value;
        auto in = new Integer;
        in->Value = --v;
        return in;
    }
    return nullptr;
}

Object *evalInfixExpression(const std::string &op, Object *l, Object *r) {
    if (l->Type() == ObjectTypes[_INTEGER_OBJ] && r->Type() == ObjectTypes[_INTEGER_OBJ]) {
        return evalIntegerInfixExpression(op, l, r);
    }

    if (l->Type() == ObjectTypes[_STRING_OBJ] && r->Type() == ObjectTypes[_STRING_OBJ]) {
        return evalStringInfixExpression(op, l, r);
    }

    if (op == "==") {
        return nativeBoolToBooleanObject(l->Inspect() == r->Inspect());
    }

    if (op == "!=") {
        return nativeBoolToBooleanObject(l->Inspect() != r->Inspect());
    }

    if (l->Type() != r->Type()) {
        return newError("type mismatch: %s %s %s", l->Type().c_str(),
                        op.c_str(), r->Type().c_str());
    }

    return newError("unknown operator: %s %s %s", l->Type().c_str(),
                    op.c_str(), r->Type().c_str());
}

Object *evalIntegerInfixExpression(const std::string &op, Object *l, Object *r) {
    auto le = dynamic_cast<Integer *>(l);
    auto ri = dynamic_cast<Integer *>(r);

    if (le != nullptr && ri != nullptr) {
        auto lv = le->Value;
        auto rv = ri->Value;
        if (op == "+") {
            return new Integer(lv + rv);
        }
        if (op == "-") {
            return new Integer(lv - rv);
        }
        if (op == "*") {
            return new Integer(lv * rv);
        }
        if (op == "/") {
            return new Integer(lv / rv);
        }

        if (op == "<") {
            return new Bool(lv < rv);
        }
        if (op == ">") {
            return new Bool(lv > rv);
        }
        if (op == "<=") {
            return new Bool(lv <= rv);
        }
        if (op == ">=") {
            return new Bool(lv >= rv);
        }
        if (op == "==") {
            return new Bool(lv == rv);
        }
        if (op == "!=") {
            return new Bool(lv != rv);
        }
        return newError("unknown operator: %s %s %s", l->Type().c_str(),
                        op.c_str(), r->Type().c_str());
    }
    return &NIL;
}

Object *evalStringInfixExpression(const std::string &op, Object *l, Object *r) {
    auto le = dynamic_cast<String *>(l);
    auto ri = dynamic_cast<String *>(r);

    if (le != nullptr && ri != nullptr) {
        auto lv = le->Value;
        auto rv = ri->Value;
        if (op == "+") {
            return new String(lv + rv);
        }
        if (op == "<") {
            return new Bool(lv < rv);
        }
        if (op == ">") {
            return new Bool(lv > rv);
        }
        if (op == "<=") {
            return new Bool(lv <= rv);
        }
        if (op == ">=") {
            return new Bool(lv >= rv);
        }
        if (op == "==") {
            return new Bool(lv == rv);
        }
        if (op == "!=") {
            return new Bool(lv != rv);
        }
        return newError("unknown operator: %s %s %s", l->Type().c_str(),
                        op.c_str(), r->Type().c_str());
    }
    return &NIL;
}

Object *evalIndexExpression(Object *l, Object *i) {
    if (l->Type() == ObjectTypes[_ARRAY_OBJ] && i->Type() == ObjectTypes[_INTEGER_OBJ]) {
        return evalArrayIndexExpression(l, i);
    }
    if (l->Type() == ObjectTypes[_HASH_OBJ]){
       return evalHashIndexExpression(l,i);
    }
    return newError("index operator not supported: %s", l->Type().c_str());
}

Object *evalArrayIndexExpression(Object *arr, Object *idx) {
    auto arrObj = dynamic_cast<Array *>(arr);
    auto inte = dynamic_cast<Integer *>(idx);
    auto ind = inte->Value;
    auto max = arrObj->Elements.size() - 1;
    if (ind < 0 || ind > max) {
        return &NIL;
    }
    return arrObj->Elements[ind];
}

Object *evalHashIndexExpression(Object *arr, Object *idx) {
    auto hashObj = dynamic_cast<Hash *>(arr);
    auto key = dynamic_cast<Hashable *>(idx);
    if (key == nullptr){
        return newError("unusable as hash key: %s",idx->Type().c_str());
    }
    auto o = new HashKey;
    *o = key->HashKey();
    auto b = hashObj->Pairs.find(o->String());
    if (b != hashObj->Pairs.end()){
        return b->second->Value;
    }
    return &NIL;
}

Object *evalHashLiteral(HashLiteral *mh, environment *env) {
    std::map<std::string, HashPair *> pairs;

    auto b = mh->Pairs.begin();
    while (b != mh->Pairs.end()) {
        auto key = Eval((*b).first, env);
        if (isError(key)) {
            return key;
        }
        auto hashKey = dynamic_cast<Hashable *>(key);
        if (hashKey == nullptr) {
            return newError("unusable as hash key: %s", key->Type().c_str());
        }
        auto value = Eval((*b).second, env);
        if (isError(value)) {
            return value;
        }
        auto hashed = new HashKey;
        *hashed = hashKey->HashKey();
        auto hp = new HashPair;
        hp->Key = key;
        hp->Value = value;
        pairs[hashed->String()] = hp;
        b++;
    }
    auto h = new Hash;
    h->Pairs = pairs;
    return h;
}

Error *newError(const char *format, ...) {
    char buf[4096];

    va_list args;
    va_start(args, format);
    vsnprintf(buf, 4096, format, args);
    va_end(args);

    std::string str = buf;
    auto e = new Error;
    e->Message = str;
    return e;
}

Object *nativeBoolToBooleanObject(bool in) {
    if (in) {
        return &TRUE;
    }
    return &FALSE;
}

bool isError(Object *obj) {
    if (obj != nullptr) {
        return obj->Type() == ObjectTypes[_ERROR_OBJ];
    }
    return false;
}

Object *applyFunction(Object *fn, std::vector<Object *> &args) {
    auto f = dynamic_cast<Function *>(fn);
    if (f != nullptr) {
        auto extendEnv = extendFunctionEnv(f, args);
        auto ev = Eval(f->Body, extendEnv);
        return unwrapReturnValue(ev);
    }
    auto b = dynamic_cast<Builtin *>(fn);
    if (b != nullptr) {
        return b->fn(args);
    }
    return newError("not a function: %s", fn->Type().c_str());

}

environment *extendFunctionEnv(Function *fn, std::vector<Object *> &args) {
    auto env = new environment(fn->Env);
    for (int i = 0; i < (args.end() - args.begin()); ++i) {
        env->Set(fn->Parameters[i]->value, args[i]);
    }
    return env;
}

Object *unwrapReturnValue(Object *obj) {
    auto re = dynamic_cast<ReturnValue *>(obj);
    if (re != nullptr) {
        return re->Value;
    }
    return obj;
}