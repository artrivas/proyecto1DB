
#pragma once

#include <iostream>

struct Player {
    long id;
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
    
    Player();
    Player(long p_id, const char* p_fname, const char* p_lname, const char* p_position,
       const char* p_height, int p_weight, const char* p_birthday, const char* p_country,
       const char* p_school, int p_draft_year, int p_draft_round, int p_draft_number);
    void print();
    long get_key();
    friend std::istream& operator>>(std::istream& is, Player& player);  
};

// Record get_record(const std::string& filename);