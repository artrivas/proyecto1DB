//
// Created by diego on 24/09/2023.
//

#ifndef PROYECTO1DB_MAIN_MENU_H
#define PROYECTO1DB_MAIN_MENU_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "sequential/src/SequentialFile.hpp"
#include "exthash/src/ExtHashFile.hpp"
#include "avl/src/AVLFile.hpp"
#include "DataManager.hpp"
#include <map>
using namespace std;
class Menu {
    map<string,string> indicesname;

    void display(){
    }
    template <typename TR,typename TK>

    void inputstatement(DataManager<TR,TK>& datamanager){
        string statement;
        cin >> statement;
        if (iscorrect(statement)){
            vector<string> instrucciones = Parser::evaluate(statement)
            calldatamanager(datamanager,instrucciones,indicesname[instrucciones.tablename]);
        }
        else{
            cout << " Stament incorrecto " << endl ;
        }

    }
    template <typename TR,typename TK>

    void calldatamanager (DataManager<TR,TK>& datamanager,vector<string> instrucciones,string index){
        if (instrucciones[0] == "SELECT" ){
            select(datamanager,index);}
        else if(instrucciones[0] == "CREATE"){
            create(datamanager,index);}
        else if(instrucciones[0] == "INSERT"){
            insert(datamanager,index);}
        else if(instrucciones[0] == "DELETE"){
            remove(datamanager,index);}
    }

    bool iscorrect(string statement ){
        return Parser::iscorrect(statement);
    }




};
#endif //PROYECTO1DB_MAIN_MENU_H
