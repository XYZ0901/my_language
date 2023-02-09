//
// Created by 周诣鑫 on 2022/10/21.
//

#include "object.h"

std::string Integer::Inspect() {
    std::ostringstream buf;
    buf << Value;
    return buf.str();
}

std::string Bool::Inspect() {
    std::ostringstream buf;
    buf << (Value ? "TRUE" : "FALSE");
    return buf.str();
}

Object *environment::Get(const std::string &name, bool &b) {
    b = store.find(name)->second;
    if (b){
        return store[name];
    }
    if (outer != nullptr){
        return outer->Get(name,b);
    }
    return nullptr;
}

Object *environment::Set(const std::string &name, Object *obj) {
    store[name] = obj;
    return obj;
}

std::string Function::Inspect() {
    std::ostringstream buf;
    buf << "fn(";
    if (!Parameters.empty()){
        buf << Parameters[0]->String();
        auto b = Parameters.begin();
        while(++b != Parameters.end()){
            buf<< ", "<<(*b)->String();
        }
    }
    buf << ") {\n";
    buf << Body->String() << std::endl << "}";

    return buf.str();
}
