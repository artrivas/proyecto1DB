
#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>

class Parser {
public:
    std::string command;
    std::vector<std::string> tokens;

public:
    Parser();
    Parser(std::string sqlStatement);

    void setStatement(std::string sqlStatement);
    void tokenize();

    std::map<std::string, std::string> getInstruction();
    std::vector<std::string> getFields(const std::string& input);
    std::string getTypeIndex(const std::string& input);
    std::string toLowerCase(std::string str);
    
};

// Implementation of Parser

Parser::Parser() {}

Parser::Parser(std::string sqlStatement) {
    command = sqlStatement;
    tokenize();
}

void Parser::setStatement(std::string sqlStatement) {
    command = sqlStatement;
    tokenize();
}

void Parser::tokenize() {
    std::istringstream iss(command);
    std::string token;
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);
    while (iss >> token) {
        tokens.push_back(token);
    }
}

std::map<std::string, std::string> Parser::getInstruction() {
    std::map<std::string, std::string> instructions;
    instructions.clear();
    
    if (tokens.size() >= 9 && tokens[0] == "create" && tokens[1] == "table") {
        instructions["operation"]   = "create";
        instructions["table_name"]  = toLowerCase(tokens[2]);
        instructions["file_path"]   = tokens[5];
        instructions["index_field"] = toLowerCase(getFields(tokens[8])[0]);
        instructions["type_index"]  = toLowerCase(getTypeIndex(tokens[8]));
    }
    else if (tokens.size() >= 8 && tokens[0] == "select" && tokens[1] == "*" && tokens[6] == "=") {
        instructions["operation"]   = "select";
        instructions["table_name"]  = toLowerCase(tokens[3]);
        instructions["index_field"] = toLowerCase(tokens[5]);
        instructions["operator"]    = toLowerCase(tokens[6]);
        instructions["value"]       = toLowerCase(tokens[7]);
    }
    else if (tokens.size() >= 10 && tokens[0] == "select" && tokens[1] == "*" && tokens[6] == "between") {
        instructions["operation"]   = "select";
        instructions["table_name"]  = toLowerCase(tokens[3]);
        instructions["operator"]    = toLowerCase(tokens[6]);
        instructions["lower_limit"] = toLowerCase(tokens[7]);
        instructions["upper_limit"] = toLowerCase(tokens[9]);
    }
    else if (tokens.size() >= 7 && tokens[0] == "insert" && tokens[1] == "into") {
        instructions["operation"]   = "insert";
        instructions["table_name"]  = toLowerCase(tokens[2]);
        instructions["values"]      = command;
    }
    else if (tokens.size() >= 7 && tokens[0] == "delete" && tokens[1] == "from") {
        instructions["operation"]   = "delete";
        instructions["table_name"]  = toLowerCase(tokens[2]);
        instructions["value"]       = tokens[6];
        instructions["index_field"] = toLowerCase(tokens[4]);
    }
    else {
        std::cout << "\n*** Sentencia no soportada o no valida ***" << std::endl;
    }
    return instructions;
}

std::vector<std::string> Parser::getFields(const std::string& input) {
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

std::string Parser::getTypeIndex(const std::string& input) {
    size_t init = input.find('(');

    if (init != std::string::npos && init > 0) 
        return input.substr(0, init);
    
    return "";
}

std::string Parser::toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}