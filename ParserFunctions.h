//
// Created by artrivas on 9/5/23.
//

#ifndef UNTITLED_PARSERFUNCTIONS_H
#define UNTITLED_PARSERFUNCTIONS_H
#include<iostream>
#include<iostream>
#include<fstream>
#include<sstream>
#include <cstring>
#include <vector>
#include "./records/Record.h"
vector<Record> CsvToBin(const std::string & csvfile);
void CsvToBinTwo(const std::string & csvfile,const std::string & binfile);

#endif //UNTITLED_PARSERFUNCTIONS_H
