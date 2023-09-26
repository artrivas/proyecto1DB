//
// Created by artrivas on 9/26/23.
//

#ifndef UNTITLED1_SCANNER_H
#define UNTITLED1_SCANNER_H
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include<unordered_map>
#include "Token.h"

using namespace std;


class Scanner {
public:
    Scanner(string in_s);
    Token* nextToken();
    ~Scanner();
private:
    string input;
    int first, current;
    int state;
    unordered_map<string, Token::Type> reserved;
    char nextChar();
    void rollBack();
    void startLexema();
    void incrStartLexema();
    string getLexema();
};


#endif //UNTITLED1_SCANNER_H
