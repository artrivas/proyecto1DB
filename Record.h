//
// Created by artrivas on 9/4/23.
//

#ifndef UNTITLED_RECORD_H
#define UNTITLED_RECORD_H
#include<iostream>
using namespace std;
struct Record{
    long playerid;
    char fname[20]{};
    char lname[20]{};
    char position[20]{};
    char height[5]{};
    int weight;
    char birthday[11]{};
    char country[15]{};
    char school[25]{};
    int draft_year;
    int draft_round;
    int draft_number;
    Record();
    void print_vals();
};

Record get_record(const string& filename);
#endif //UNTITLED_RECORD_H
