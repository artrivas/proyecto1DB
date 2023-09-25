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

void Record::set_vals() {
    cout << "Player Id: \n";
    cin >> playerid;
    cout << "Fist Name: \n";
    cin >> fname;
    cout << "Last Name: \n";
    cin >> lname;
    cout << "Position: \n";
    cin >> position;
    cout << "Height: \n";
    cin >> height;
    cout << "Weight: \n";
    cin >> weight;
    cout << "Birthday: \n";
    cin >> birthday;
    cout << "country: \n";
    cin >> country;
    cout << "school: \n";
    cin >> school;
    cout << "draft year: \n";
    cin >> draft_year;
    cout << "draft round: \n";
    cin >> draft_round;
    cout << "draft number: \n";
    cin >> draft_number;
}

vector<long> Record::get_prefix(){
    vector<long> ans;
    long sum = 0;
    ans.push_back(sum);
    sum+=sizeof(this->playerid);
    ans.push_back(sum);
    sum+=sizeof(this->fname);
    ans.push_back(sum);
    sum+=sizeof(this->lname);
    ans.push_back(sum);
    sum+=sizeof(this->position);
    ans.push_back(sum);
    sum+=sizeof(this->height);
    ans.push_back(sum);
    sum+=sizeof(this->weight);
    ans.push_back(sum);
    sum+=sizeof(this->birthday);
    ans.push_back(sum);
    sum+=sizeof(this->country);
    ans.push_back(sum);
    sum+=sizeof(this->school);
    ans.push_back(sum);
    sum+=sizeof(this->draft_year);
    ans.push_back(sum);
    sum+=sizeof(this->draft_round);
    ans.push_back(sum);
    return ans;
}
