
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

class Parser {
private:
    std::string command;
    std::vector<std::string> tokens;

public:
    Parser(std::string sqlStatement) {
        command = sqlStatement;
        tokenize();
    }

    void tokenize() {
        std::istringstream iss(command);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
    }

    std::vector<std::string> getFields(const std::string& comando) {
        std::vector<std::string> valores;

        // Encontrar la parte entre paréntesis
        size_t inicio = comando.find('(');
        size_t fin = comando.find(')');
        if (inicio != std::string::npos && fin != std::string::npos && fin > inicio) {
            std::string contenido = comando.substr(inicio + 1, fin - inicio - 1);

            // Leer valores separados por comas
            std::istringstream iss(contenido);
            std::string valor;
            while (std::getline(iss, valor, ',')) {
                // Eliminar espacios en blanco al inicio y al final
                valor = valor.substr(valor.find_first_not_of(" "), valor.find_last_not_of(" ") + 1);
                
                // Si el valor es un string, eliminar comillas simples
                if (valor.front() == '\'' && valor.back() == '\'') {
                    valor = valor.substr(1, valor.length() - 2);
                }

                valores.push_back(valor);
            }
        }

        return valores;
    }

    std::string getValue(const std::string& texto) {
        size_t inicio = texto.find('(');

        if (inicio != std::string::npos && inicio > 0) 
            return texto.substr(0, inicio);
        return "";
    }

    std::map<std::string, std::string> getInstruccion() {
        std::map<std::string, std::string> instruccions;
        
        if (tokens[0] == "create" && tokens[1] == "table") {  
            instruccions["connector"] = "create";
            instruccions["tableName"] = tokens[2];
            instruccions["filePath"] = tokens[5];
            instruccions["indexInfo"] = tokens[8];
            instruccions["fieldIndex"] = getFields(tokens[8])[0];
            instruccions["typeIndex"] = getValue(tokens[8]);
            
        }
        else if (tokens[0] == "select" && tokens[1] == "*") {
            instruccions["connector"] = "select";
            instruccions["tableName"] = tokens[3];
            instruccions["fieldCondition"] = tokens[5];
        
        }
        else if (tokens[0] == "select" && tokens[1] == "*" && tokens[6] == "between") {
            instruccions["connector"] = "select";
            instruccions["tableName"] = tokens[3];
            instruccions["lowerBound"] = tokens[7];
            instruccions["upperBound"] = tokens[9];
          
        }

        else if (tokens[0] == "insert" && tokens[1] == "into") {
            instruccions["connector"] = "insert";
            instruccions["tableName"] = tokens[2];
            instruccions["fields"] = command;
        }
        else if (tokens[0] == "delete" && tokens[1] == "from") {
            instruccions["connector"] = "create";
            instruccions["tableName"] = tokens[2];
            instruccions["field"] = tokens[4];
            instruccions["value"] = tokens[6];
        }
        else {
            std::cout << "Comando no soportado o no válido" << std::endl;
        }
        return instruccions;
    }


    // ----------------------------------------------------------------
    void createHash(std::string filePath, std::string field_index) {
        

    }
};