//
// Created by 周诣鑫 on 2022/10/18.
//

#include "token.h"

TokenType LookupIdent(const std::string &ident) {
    auto tokIterator = keywords.find(ident);
    if (tokIterator != keywords.end()) {
        return tokIterator->second;
    }
    return tokenCodes[_IDENT];
}