
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "sequential/src/SequentialFile.hpp"
#include "exthash/src/ExtHashFile.hpp"
#include "avl/src/AVL.h"

#include "DataManager.hpp"
#include "Parser.hpp"

using namespace std;

template <typename TR, typename TK>
class Menu {
    map<string, string> indices_name;
    DataManager<TR, TK>& data_manager;
    Parser& parser;

public:
    Menu(DataManager<TR, TK>& data_manager);

    void display();

    void create(string datafile, string index, string field_index);
    void insert(string tablename, string data);
    void select(string tablename);
    void remove(string datafile, string index, string field_index);

    void input_statement();
    bool execute(vector<string> tokens);
    bool evaluate(map<string, string> instruccions);



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

// ----------------------------------------------------------------------------------------
template <typename TR, typename TK>
Menu<TR, TK>::Menu(DataManager<TR, TK>& datamanager) : datamanager(datamanager) {}

template <typename TR, typename TK>
void Menu<TR, TK>::display() {
    cout << "BIENVENIDOS A MI SGBD\n\n";
    cout << ">> Inserte su sentencia SQL: \n";
    input_statement();
}

template <typename TR, typename TK>
void Menu<TR, TK>::create(string datafile, string index, string field_index) {
    if (index == "Hash") 
        datamanager.createhashindice(datafile, field_index);
    else if (index == "avl") 
        datamanager.createavlindice(datafile, field_index);
    else if (index == "sequential") 
        datamanager.create_sequential_indice(datafile, field_index);
}

template <typename TR, typename TK>
void Menu<TR, TK>::insert(string tablename, string data) {
    string index = indices_name[tablename];
    if (index == "hash") {
        auto& ehash = data_manager.hasindices[0];
        std::vector<std::string> fields = getFields(data);
        TR record(fields); 
        ehash.add(record);
    }
    else if (index == "avl") {
        auto& avlf = data_manager.avlindices[0];
        std::vector<std::string> fields = getFields(data);
        TR record(fields); 
        avlf.insert(record);
    } 
    else if (index == "sequential") {
        auto& seqf = data_manager.sequentialindices[0];
        std::vector<std::string> fields = getFields(data);
        TR record(fields); 
        seqf.insert(record);
    }
}

template <typename TR, typename TK>
void Menu<TR, TK>::select(string datafile) {
    string index = indices_name["datafile"];
    if (index == "hash") {
        auto& ehash = data_manager.hasindices[0];
        e.hash.print();    
    }
    else if (index == "avl") {
        auto& ehash = data_manager.hasindices[0];

    }
    else if (index == "sequential") {

    }
    



}

template <typename TR, typename TK>
void Menu<TR, TK>::remove() {

}

template <typename TR, typename TK>
void Menu<TR, TK>::input_statement(){
    string statement;
    std::getline(std::cin, statement);
   
    if (statement.size() == 0)
        return;
    
    auto instruccion = parser.getInstruccion(statement);

    if (instruccion.is_empty()) 
        cout << "Error\n";
    else {
        evaluate(instruccion);
        cout << "Ejecucion exitosa\n";
    }
}


    //calldatamanager(datamanager,instrucciones,indicesname[instrucciones.tablename]);




// ----------------------------------------------------------------

template <typename TR, typename TK>
bool Menu<TR, TK>::evaluate(map<string, string> instruccions) {   
    if (instruccions["connector"] == "create") {  
        create(instruccions["filePath"], instruccions["typeIndex"], instruccions["typeIndex"]);
        indices_name["filePath"] = instruccions["typeIndex"];
    } 
    else if (instruccions["connector"] == "select") {
        select(instruccions["tableName"]);
    } 
    else if (instruccions["connector"] == "insert") {
        insert(instruccions["tableName"], instruccions["fields"]);
    } 
    else if (instruccions["connector"] == "delete") {
        remove(instruccions["filePath"], instruccions["typeIndex"], instruccions["typeIndex"]);

        
    }

        std::string tableName = tokens[2];
        std::string filePath = tokens[5]; 
        std::string indexInfo = tokens[8]; 

        std::vector<std::string> field_index = getFields(indexInfo);
        std::string type_index = getValue(indexInfo);

        if (type_index == "hash") {
            createHash(filePath, field_index);

        } else if (type_index == "avl") {
            createAvl(filePath, field_index);

        } else if (type_index == "sequential") {
            createSequential(filePath, field_index);
        }

    }
    else if (tokens[0] == "select" && tokens[1] == "*") {
        // Implementar lógica para select all
        std::string tableName = tokens[3];
        std::string condition = tokens[5];
    }
    else if (tokens[0] == "select" && tokens[1] == "*" && tokens[6] == "between") {
        // Implementar lógica para select con BETWEEN
        std::string tableName = tokens[3];
        std::string lowerBound = tokens[7];
        std::string upperBound = tokens[9];
    }
    else if (tokens[0] == "insert" && tokens[1] == "into") {
        // Implementar lógica para insertar
        std::string tableName = tokens[2];
        auto fields = getFields(command);

    }
    else if (tokens[0] == "delete" && tokens[1] == "from") {
        // Implementar lógica para eliminar
        std::string tableName = tokens[2];
        std::string field = tokens[4];
        std::string value = tokens[6];
    }
    else {
        std::cout << "Comando no soportado o no válido" << std::endl;
        return false;
    }
    return true;
}


// ----------------------------------------------------------------
void createHash(std::string filePath, std::string field_index) {
    

}


        auto fields = getFields(command);