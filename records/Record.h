//
// Created by artrivas on 9/4/23.
//

#ifndef UNTITLED_RECORD_H
#define UNTITLED_RECORD_H
#include<iostream>
#include<vector>
using namespace std;
struct Record{
    long playerid;
    char fname[20]{};
    char lname[20]{};
    char position[20]{};
    char height[15]{};
    int weight;
    char birthday[15]{};
    char country[15]{};
    char school[25]{};
    int draft_year;
    int draft_round;
    int draft_number;
    Record();
    void print_vals();
    long getPrimaryKey() const;
    void set_vals(long pid,char fn[],char ln[], char pos[],char h[], int w, char birth[],char cou[], char scho[],int dy,int dr,int dn);
    vector<long> get_prefix();
};

Record get_record(const string& filename);
#endif //UNTITLED_RECORD_H
