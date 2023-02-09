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
    auto i = store.find(name);
    b = (i != store.end());
    if (b) {
        return store[name];
    }
    if (outer != nullptr) {
        return outer->Get(name, b);
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
    if (!Parameters.empty()) {
        buf << Parameters[0]->String();
        auto b = Parameters.begin();
        while (++b != Parameters.end()) {
            buf << ", " << (*b)->String();
        }
    }
    buf << ") {\n";
    buf << Body->String() << std::endl << "}";

    return buf.str();
}

std::string Array::Inspect() {
    std::ostringstream buf;

    buf << "[";
    if (!Elements.empty()) {
        buf << Elements[0]->Inspect();
        auto b = Elements.begin();
        while (++b != Elements.end()) {
            buf << ", " << (*b)->Inspect();
        }
    }
    buf << "]";

    return buf.str();
}

std::string Hash::Inspect() {
    std::ostringstream buf;

    buf << "{";
    if (!Pairs.empty()) {
        auto b = Pairs.begin();
        buf << (*b).second->Key->Inspect() << ": " << (*b).second->Value->Inspect();
        while (++b != Pairs.end()) {
            buf << ", " << (*b).second->Key->Inspect() << ": " << (*b).second->Value->Inspect();
        }
    }
    buf << "}";

    return buf.str();
}
