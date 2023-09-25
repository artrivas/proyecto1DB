
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

    map<string, vector<string>> indexes_name{};
    DataManager<TR, TK>& data_manager;
    Parser& parser;

public:

    Menu(DataManager<TR, TK>& data_manager, Parser& parser);
    ~Menu();

    void display_main();
    void display_input_statement();
    void execute(map<string, string>& instructions);

    void create(map<string, string>& instructions);
    void insert(map<string, string>& instructions);
    void select(map<string, string>& instructions);
    void remove(map<string, string>& instructions);

    std::vector<std::string> getValues(const std::string& input);
    bool contains(std::vector<std::string>& indexes, std::string index); 
    void save_indexes_name(const std::map<std::string, std::vector<std::string>>& mapa, const std::string& nombre_archivo);
    std::map<std::string, std::vector<std::string>> load_indexes_name(const std::string& nombre_archivo);

    void clear();
    void pause(); 
};

// Implementation of Menu

template <typename TR, typename TK>
Menu<TR, TK>::Menu(DataManager<TR, TK>& data_manager, Parser& parser) : data_manager(data_manager), parser(parser) {
    indexes_name = load_indexes_name("index/exthash/indexes_name.txt");
    std::cout << "INDEXES LOADED" << std::endl;
    for (auto& [k, v]: indexes_name) {
        std::cout << k << ": "; 
        for (auto& p: v) 
            std::cout << p << " ";
        std::cout << std::endl;
    }
}

template <typename TR, typename TK>
Menu<TR, TK>::~Menu() {
    save_indexes_name(indexes_name, "index/exthash/indexes_name.txt");
    std::cout << "INDEXES SAVED" << std::endl;
    for (auto& [k, v]: indexes_name) {
        std::cout << k << ": "; 
        for (auto& p: v) 
            std::cout << p << " ";
        std::cout << std::endl;
    }
}


template <typename TR, typename TK>
void Menu<TR, TK>::display_main() {
    cout << "\n---------------------------------------------\n";
    cout << "----------- Bienvenido a MySGDB -------------\n";
    cout << "---------------------------------------------\n";
    cout << "\n\t[1] Insertar una sentencia SQL\n";
    cout << "\t[2] Cerrar sesion\n";
    
    int option;
    do {
        cout << "\n>> Elije una opcion: ";
        cin >> option;
    } while (1 > option || option > 2);

    cout << "\n\n";
    clear();

    switch (option) {
    case 1:
        display_input_statement();
        break;
    case 2:
        cout << "** Gracias siuuuu! :) **\n";
        break;
    }
}

template <typename TR, typename TK>
void Menu<TR, TK>::display_input_statement() {
    std::string statement{};
    std::map<std::string, std::string> instruction;
    
    std::cout << "** Puedes presionar 0 para salir al menu principal **\n";
    do {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        cin.clear();
        cout << "\n:: Inserte su sentencia SQL: \n\n>> ";
        std::getline(std::cin, statement);

        if (statement == "0") break;
        parser.setStatement(statement);
        instruction = parser.getInstruction();
    } while (statement.size() == 0 || instruction.empty() == true);
    
    if (statement == "0") {
        pause();
        clear();
        display_main();
        return;
    }

    execute(instruction); // se considera valido
    // cout << "\n*** Ejecucion exitosa :) ***\n";
    
    pause();
    clear();
    display_main();
}

template <typename TR, typename TK>
void Menu<TR, TK>::execute(map<string, string>& instructions) {
    if (instructions["operation"] == "create") 
        create(instructions);
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
            data_manager.insert_hash_index(instructions["table_name"], record);
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
            data_manager.select_hash_index(stoi(instructions["value"]), instructions["table_name"], instructions["index_field"]); 
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
            data_manager.remove_hash_index(stoi(instructions["value"]), instructions["table_name"], instructions["index_field"]);
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
void Menu<TR, TK>::save_indexes_name(const std::map<std::string, std::vector<std::string>>& mapa, const std::string& nombre_archivo) {
    std::ofstream archivo(nombre_archivo);

    if (archivo.is_open()) {
        for (const auto& entry : mapa) {
            archivo << entry.first << ": ";
            for (const auto& valor : entry.second) {
                archivo << valor << " ";
            }
            archivo << "\n";
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para escritura." << std::endl;
    }
}

template <typename TR, typename TK>
std::map<std::string, std::vector<std::string>> Menu<TR, TK>::load_indexes_name(const std::string& nombre_archivo) {
    std::map<std::string, std::vector<std::string>> mapa;
    std::ifstream archivo(nombre_archivo);

    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::string clave;
            std::vector<std::string> valores;

            size_t pos = linea.find(": ");
            if (pos != std::string::npos) {
                clave = linea.substr(0, pos);
                std::string valores_str = linea.substr(pos + 2);

                size_t inicio = 0;
                size_t fin = valores_str.find(" ");

                while (fin != std::string::npos) {
                    valores.push_back(valores_str.substr(inicio, fin - inicio));
                    inicio = fin + 1;
                    fin = valores_str.find(" ", inicio);
                }

                valores.push_back(valores_str.substr(inicio));
                mapa[clave] = valores;
            }
        }

        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para lectura" << std::endl;
    }

    return mapa;
}

template <typename TR, typename TK>
void Menu<TR, TK>::clear() {
    system("cls");
}

template <typename TR, typename TK>
void Menu<TR, TK>::pause() {
    std::cout << "\nPresiona Enter para continuar...";
    getch();
    std::cout << "\n\n";
}