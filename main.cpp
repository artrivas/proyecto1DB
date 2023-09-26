#include<iostream>
#include "Scanner.h"
#include "Parser.h"
#include "./avl/AVL.h"
#include "ParserFunctions.h"
using namespace std;


int main() {
    Parser parser;
    cout << "PROGRAMA\n";
    while(true){
        cout << "INGRESE SU SENTENCIA: \n";
        string input;
        getline(cin, input);
        Scanner * scanner = new Scanner(input);
        parser.changeScanner(scanner);
        parser.parse();
    }
}


