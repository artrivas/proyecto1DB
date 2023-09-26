//
// Created by artrivas on 9/26/23.
//

#ifndef UNTITLED1_PARSER_H
#define UNTITLED1_PARSER_H
#include "Scanner.h"
#include "Token.h"
#include<vector>
#include<map>
#include "./avl/AVL.h"
#include "ParserFunctions.h"
#include<cstring>
#include <stdio.h>
#include <string.h>
#include "./records/Record.h"
class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    map<string,int> atributos;
    map<string,string> tablas;
    map<string,string> get_attribute;
    map<string,AVLFile<char[20]> *> avl20;
    map<string,AVLFile<char[15]>*> avl15;
    map<string,AVLFile<char[25]>*> avl25;
    map<string,AVLFile<long>*> avl8;
    map<string,AVLFile<int>*> avl4;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();

public:
    explicit Parser();
    void changeScanner(Scanner * sc);
    void parse();
    void icreate();
    void iinsert();
    void iselect();
    void idelete();
};

#endif //UNTITLED1_PARSER_H
