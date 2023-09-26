//
// Created by artrivas on 9/26/23.
//

#include "Token.h"
const char* Token::token_names[23] = { "INSTRUCTION", "CONNECTION", "TABLE", "FILE", "ERR", "END", "LOCATION", "USING",
                                       "INDEXNAME", "ATTRIBUTES", "EQUAL", "BETWEEN", "VALUES", "ID", "AND", "COMPARE", "NUM",
                                       "ADD", "SUB", "MUL", "DIV", "STORE", "LOAD" };

Token::Token(Type type):type(type) { lexema = ""; }

Token::Token(Type type, char c):type(type) { lexema = c; }

Token::Token(Type type, const string source):type(type) {
    lexema = source;
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    if (tok.lexema.empty())
        return outs << Token::token_names[tok.type];
    else
        return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}
