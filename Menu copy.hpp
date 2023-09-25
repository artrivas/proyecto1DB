
#pragma once

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <conio.h>
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
private:

    map<string, vector<string>> indexes_name;
    DataManager<TR, TK>& data_manager;
    Parser& parser;

public:

    Menu(DataManager<TR, TK>& data_manager, Parser& parser);

    void display();
    void input_statement();
    void execute(map<string, string>& instructions);

    void create(map<string, string>& instructions);
    void insert(map<string, string>& instructions);
    void select(map<string, string>& instructions);
    void remove(map<string, string>& instructions);

    std::vector<std::string> getValues(const std::string& input);
    bool contains(std::vector<std::string>& indexes, std::string index); 

    void clear();
    void pause(); 
};

// Implementation of Menu

template <typename TR, typename TK>
Menu<TR, TK>::Menu(DataManager<TR, TK>& data_manager, Parser& parser) : data_manager(data_manager), parser(parser) {}

template <typename TR, typename TK>
void Menu<TR, TK>::display() {
    cout << "\n---------------------------------------------\n";
    cout << "----------- Bienvenido a MySGDB -------------\n";
    cout << "---------------------------------------------\n";
    input_statement();
}

template <typename TR, typename TK>
void Menu<TR, TK>::input_statement(){
    string statement;
    std::map<std::string, std::string> instruction;
    
    do {
        cout << "\n:: Inserte su sentencia SQL: \n\n>> ";
        std::getline(std::cin, statement);
        parser.setStatement(statement);
        instruction = parser.getInstruction();
        std::cout << "::::::::" << instruction.size() << endl;
        //
        for (auto& [k, v]: instruction){
            std::cout << k << " : " << v << '\n';
        }
        //
    } while (statement.size() == 0 || instruction.empty() == true);

    pause();
    clear();

    execute(instruction); // se considera valido
    cout << "\n*** Ejecucion exitosa :)\n";
}

template <typename TR, typename TK>
void Menu<TR, TK>::execute(map<string, string>& instructions) {
    if (instructions["operation"] == "create") {
        std::cout << "ENTROOOOOOOOOOO2\n"; //////
        create(instructions);
    }
    else if (instructions["operation"] == "insert") 
        insert(instructions);
    else if (instructions["operation"] == "select") 
        select(instructions);
    else if (instructions["operation"] == "delete")
        remove(instructions);
}

template <typename TR, typename TK>
void Menu<TR, TK>::create(map<string, string>& instructions) {
    if (instructions["type_index"] == "hash") {
        data_manager.create_hash_index(instructions["table_name"], instructions["index_field"], instructions["file_path"]);
        indexes_name["table_name"].push_back(instructions["type_index"]);
    }
    else if (instructions["type_index"] == "avl") {
        // data_manager.create_avl_index(index_filename, heap_filename); // completar
        // indexes_name["table_name"].push_back(instructions["type_index"]);
    }
    else if (instructions["type_index"] == "sequential") { 
        // data_manager.create_seq_index(datafile, auxfile); // completar
        // indexes_name["table_name"].push_back(instructions["type_index"]);
    }
}

template <typename TR, typename TK>
void Menu<TR, TK>::insert(map<string, string>& instructions) {
    std::vector<std::string> indexes = indexes_name["table_name"];
    std::vector<std::string> fields = getValues(instructions["values"]);
    TR record(fields); 
    for (auto& index : indexes) {
        if (index == "hash") {
            data_manager.insert_hash_index(record);
        }
        else if (index == "avl") {
            // data_manager.insert_avl_index(record);
        } 
        else if (index == "sequential") {
            // data_manager.insert_seq_index(record);
        }
    }
}

template <typename TR, typename TK>
void Menu<TR, TK>::select(map<string, string>& instructions) {
    std::vector<std::string> indexes = indexes_name["table_name"];

    if (instructions["operator"] == "=") {
        if (contains(indexes, "hash")) {
            data_manager.select_hash_index(stoi(instructions["value"])); 
        }
        else if (contains(indexes, "avl")) {
            // data_manager.select_avl_index(stoi(instructions["value"]));
        }
        else if (contains(indexes, "sequential")) {
            // data_manager.select_seq_index(stoi(instructions["value"]));
        }
        else 
            std::cout << "\n:: Debe crear el indice previamente\n";
    }
    else if (instructions["operator"] == "between") {
        if (contains(indexes, "avl")) {
            // data_manager.select_range_avl_index(stoi(instructions["lower_limit"]), stoi(instructions["upper_limit"]));
        }
        else if (contains(indexes, "sequential")) {
            // data_manager.select_range_seq_index(stoi(instructions["lower_limit"]), stoi(instructions["upper_limit"]));
        }
        else {
            std::cout << "\n:: Debe crear el indice previamente\n";
        }
    }
}

template <typename TR, typename TK>
void Menu<TR, TK>::remove(map<string, string>& instructions) {
    std::vector<std::string> indexes = indexes_name["table_name"];
    for (auto& index : indexes) {
        if (index == "hash") {
            data_manager.remove_hash_index(stoi(instructions["value"]));
        }
        else if (index == "avl") {
            // data_manager.remove_avl_index(stoi(instructions["value"]));
        } 
        else if (index == "sequential") {
            // data_manager.remove_seq_index(stoi(instructions["value"]));
        }
    }
}

template <typename TR, typename TK>
std::vector<std::string> Menu<TR, TK>::getValues(const std::string& input) {
    std::vector<std::string> fields;

    size_t init = input.find('(');
    size_t end = input.find(')');
    if (init != std::string::npos && end != std::string::npos && end > init) {
        std::string content = input.substr(init + 1, end - init - 1);

        // Read values separated by commas
        std::istringstream iss(content);
        std::string value;
        while (std::getline(iss, value, ',')) {
            // Remove leading and trailing whitespace
            value = value.substr(value.find_first_not_of(" "), value.find_last_not_of(" ") + 1);
            
            // If the value is a string, remove single quotes
            if (value.front() == '\'' && value.back() == '\'') {
                value = value.substr(1, value.length() - 2);
            }

            fields.push_back(value);
        }
    }
    return fields;
}

template <typename TR, typename TK>
bool Menu<TR, TK>::contains(std::vector<std::string>& indexes, std::string index) {
    return std::find(indexes.begin(), indexes.end(), index) != indexes.end();
}

template <typename TR, typename TK>
void Menu<TR, TK>::clear() {
    system("cls");
}

template <typename TR, typename TK>
void Menu<TR, TK>::pause() {
    std::cout << "Presiona Enter para continuar...";
    getch();
    std::cout << "\n\n";
}
