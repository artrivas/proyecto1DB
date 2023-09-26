//
// Created by artrivas on 9/26/23.
//
#include "Scanner.h"
void Scanner::rollBack() { // retract
    current--;
}

void Scanner::startLexema() {
    first = current-1;
    return;
}

void Scanner::incrStartLexema() {
    first++;
}


string Scanner::getLexema() {
    return input.substr(first,current-first);
}

Scanner::~Scanner() { }

char Scanner::nextChar() {
    int c = input[current];
    current++;
    return c;
}
Scanner::Scanner(string s):input(s),first(0),current(0) {}

Token* Scanner::nextToken() {
    Token* token;
    char c;
    string lex;
    Token::Type ttype;
    c = nextChar();
    //cout << c << '\n';
    while (c == ' ') c = nextChar();
    if (c == '\0') return new Token(Token::END);
    startLexema();
    state = 0;
    while (true) {
        switch (state) {
            case 0: {
                //cout << "here" << ':' << c;
                if (isalpha(c)) state = 1; // PALABRAS
                else if (c == '"') state = 4; //"C:\data.csv"
                else if (c == '(') state = 9; //("DNI")
                else if (c == '*') state = 11; //*
                else if (c == '=') return new Token(Token::EQUAL,c);
                else if(c == '\'') state = 12;
                else if(isdigit(c)) state = 14; //Falta para digitos
                else state = 2; //ERROR
                break;
            }
            case 14:{
                c = nextChar();
                if(isdigit(c)) state = 14;
                else{
                    rollBack();
                    return new Token(Token::NUM,getLexema());
                }
                break;
            }
            case 12: {
                c = nextChar();
                if(c == '\'') return new Token(Token::COMPARE,getLexema());
                else state = 12;
                break;
            }
            case 13: {
                rollBack();
                string lexema = getLexema();
                string ans = "";
                for(int i = 1; i< lexema.size();i++){
                    ans+=lexema[i];
                }
                return new Token(Token::EQUAL,ans);
            }
            case 11: {
                c = nextChar();
                rollBack();
                if (c == ' ') {
                    return new Token(Token::ATTRIBUTES,getLexema());
                } else {
                    return new Token(Token::ERR);
                }
                break;
            }
            case 1: {
                c = nextChar();
                if (isalpha(c)) {
                    state = 1; //Palabra
                }
                else if (c == '(') state = 6; //values(...)
                else {
                    state = 3; //Palabra
                }
                break;
            }
            case 2: {
                return new Token(Token::ERR, c);
                break;
            }
            case 3: {
                rollBack();
                string lexema = getLexema();
                if(lexema == "create" || lexema == "select" || lexema == "insert" || lexema == "delete"){
                    return new Token(Token::INSTRUCTION,lexema);
                }else if(lexema == "into" || lexema == "from" || lexema == "where"){
                    return new Token(Token::CONNECTION,lexema);
                }else if(lexema == "table"){
                    return new Token(Token::TABLE,lexema);
                }else if(lexema == "file") {
                    return new Token(Token::FILE, lexema);
                }else if(lexema == "using"){
                    return new Token(Token::USING,lexema);
                }else if(lexema == "between"){
                    return new Token(Token::BETWEEN,lexema);
                }else if(lexema == "and"){
                    return new Token(Token::AND,lexema);
                }else{
                    return new Token(Token::ID,lexema);
                }
                break;
            }
            case 4: {
                c = nextChar();
                if (c == '"') state = 5;
                else if (c != ' ') state = 4;
                else return new Token(Token::ERR, c);
                break;
            }
            case 5: {
                string lexema = getLexema();
                string ans = "";
                for(int i = 1; i < lexema.size()-1;i++){
                    ans += lexema[i];
                }
                return new Token(Token::LOCATION, ans);
                break;
            }
            case 6:{
                rollBack();
                string lexema = getLexema();
                if(lexema == "values") state = 7;
                else return new Token(Token::INDEXNAME, getLexema());
                break;
            }
            case 7: {
                c = nextChar();
                if(c == ' ') state = 2;
                else if (c != ')') state = 7;
                else state = 8;
                break;
            }
            case 8:{
                string lexema = getLexema();
                string ans = "";
                for(int i = 7; i < lexema.size()-1;i++){
                    ans+=lexema[i];
                }
                return new Token(Token::VALUES,ans);
                break;
            }
            case 9:{
                c = nextChar();
                if(c == ' ') state = 2;
                else if(c != ')') state = 9;
                else state = 10;
                break;
            }
            case 10:{
                string lexema = getLexema();
                if(lexema.size() > 3 && lexema[1] == '"' && lexema[lexema.size()-2] == '"'){
                    string ans = "";
                    for(int i = 2; i < lexema.size()-2 ;i++){
                        ans+=lexema[i];
                    }
                    return new Token(Token::ATTRIBUTES,ans);
                }else{
                    return new Token(Token::ERR);
                }
                break;
            }
            default:
                cout << "programming error ... quitting" << endl;
                exit(0);
        }
    }
    return new Token(Token::ERR);

}

