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

void Record::set_vals(long pid,char fn[],char ln[],
                      char pos[],char h[], int w,
                      char birth[],char cou[],
                      char scho[],int dy,int dr,int dn) {
    playerid = pid;
    strcpy(fname,fn);
    strcpy(lname,ln);
    strcpy(position,pos);
    strcpy(height,h);
    weight = w;
    strcpy(birthday,birth);
    strcpy(country,cou);
    strcpy(school,scho);
    draft_year = dy;
    draft_round = dr;
    draft_number = dn;
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
