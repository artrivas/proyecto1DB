//
// Created by artrivas on 9/26/23.
//

#ifndef UNTITLED1_TOKEN_H
#define UNTITLED1_TOKEN_H
#include <iostream>
#include "Token.h"
using namespace std;
class Token {
public:
    enum Type { INSTRUCTION=0, CONNECTION, TABLE, FILE, ERR, END, LOCATION, USING, INDEXNAME, ATTRIBUTES, EQUAL, BETWEEN, VALUES, ID, AND, COMPARE, NUM, ADD, SUB, MUL, DIV,
        STORE, LOAD };
    static const char* token_names[23];
    Type type;
    string lexema;
    Token(Type);
    Token(Type, char c);
    Token(Type, const string source);
};

#endif //UNTITLED1_TOKEN_H
