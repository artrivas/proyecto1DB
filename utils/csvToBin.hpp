
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

template <typename TypeRecord>
void csvToBin(const std::string& csvfile, const std::string& binfile) {
    std::ifstream inputFile(csvfile);
    std::ofstream outputFile(binfile, std::ios::binary | std::ios::app);
    if (!inputFile.is_open())
        throw std::runtime_error("Cannot open csvfile"); 
    if (!outputFile.is_open())
        throw std::runtime_error("Cannot open binfile"); 
    
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        TypeRecord record;
        iss >> record;
        outputFile.write((char*) &record, sizeof(TypeRecord));
    }
    inputFile.close();
    outputFile.close();
}
