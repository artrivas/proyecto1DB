//
// Created by artrivas on 9/4/23.
//
#include<iostream>
#include<fstream>
#include <cstring>
#include<sstream>
#include "Record.h"
using namespace std;
Record::Record() {
    this->playerid = 0;
    this->fname[0] = 0;
    this->lname[0] = 0;
    this->position[0] = 0;
    this->height[0] = 0;
    this->weight = 0;
    this->birthday[0] = 0;
    this->country[0] = 0;
    this->school[0] = 0;
    this->draft_year = 0;
    this->draft_round = 0;
    this->draft_number = 0;
}
void Record::print_vals() {
    cout << "Player Id: "<<playerid << '\n';
    cout << "Fist Name: "<<fname << '\n';
    cout << "Last Name: "<<lname << '\n';
    cout << "Position: "<<position << '\n';
    cout << "Height: "<<height << '\n';
    cout << "Weight: "<<weight << '\n';
    cout << "Birthday: "<<birthday << '\n';
    cout << "country: "<<country << '\n';
    cout << "school: "<<school << '\n';
    cout << "draft year: "<<draft_year << '\n';
    cout << "draft round: " << draft_round << '\n';
    cout << "draft number: " << draft_number << '\n';
}
