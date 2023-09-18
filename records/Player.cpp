
#include <cstring>
#include "Player.hpp"

Player::Player() {
    this->id = 0;
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

Player::Player(long p_id, const char* p_fname, const char* p_lname, const char* p_position,
        const char* p_height, int p_weight, const char* p_birthday, const char* p_country,
        const char* p_school, int p_draft_year, int p_draft_round, int p_draft_number) {
    id = p_id;
    strncpy(fname, p_fname, sizeof(fname)-1);
    fname[sizeof(fname)-1] = '\0';
    strncpy(lname, p_lname, sizeof(lname)-1);
    lname[sizeof(lname)-1] = '\0';
    strncpy(position, p_position, sizeof(position)-1);
    position[sizeof(position)-1] = '\0';
    strncpy(height, p_height, sizeof(height)-1);
    height[sizeof(height)-1] = '\0';
    weight = p_weight;
    strncpy(birthday, p_birthday, sizeof(birthday)-1);
    birthday[sizeof(birthday)-1] = '\0';
    strncpy(country, p_country, sizeof(country)-1);
    country[sizeof(country)-1] = '\0';
    strncpy(school, p_school, sizeof(school)-1);
    school[sizeof(school)-1] = '\0';
    draft_year = p_draft_year;
    draft_round = p_draft_round;
    draft_number = p_draft_number;
}

void Player::print() {
    std::cout << "\tId: " << id << '\n';
    std::cout << "\tFist Name: " << fname << '\n';
    std::cout << "\tLast Name: " << lname << '\n';
    std::cout << "\tPosition: " << position << '\n';
    std::cout << "\tHeight: " << height << '\n';
    std::cout << "\tWeight: " << weight << '\n';
    std::cout << "\tBirthday: " << birthday << '\n';
    std::cout << "\tCountry: " << country << '\n';
    std::cout << "\tSchool: " << school << '\n';
    std::cout << "\tDraft year: " << draft_year << '\n';
    std::cout << "\tDraft round: " << draft_round << '\n';
    std::cout << "\tDraft number: " << draft_number << '\n';
}

long Player::get_key() {
    return id; // changeable
}

std::istream& operator>>(std::istream& is, Player& player) {
    std::string temp;
    // id
    std::getline(is, temp, ',');
    player.id = (temp.empty() ? -1 : stoi(temp)); 
    // fname
    std::getline(is, temp, ',');
    if (temp.empty()) temp = "NULL";
    strcpy(player.fname, temp.c_str());
    // lname
    std::getline(is, temp, ',');
    if (temp.empty()) temp = "NULL";
    strcpy(player.lname, temp.c_str());
    // position
    std::getline(is, temp, ',');
    if (temp.empty()) temp = "NULL";
    strcpy(player.position, temp.c_str());
    // height
    std::getline(is, temp, ',');
    if (temp.empty()) temp = "NULL";
    strcpy(player.height, temp.c_str());
    // weight
    std::getline(is, temp, ',');
    player.weight = (temp.empty() ? -1 : stoi(temp)); 
    // birthday
    std::getline(is, temp, ',');
    if (temp.empty()) temp = "NULL";
    strcpy(player.birthday, temp.c_str());
    // country
    std::getline(is, temp, ',');
    if (temp.empty()) temp = "NULL";
    strcpy(player.country, temp.c_str());
    // school
    std::getline(is, temp, ',');
    if (temp.empty()) temp = "NULL";
    strcpy(player.school, temp.c_str());
    // draft_year
    std::getline(is, temp, ',');
    player.draft_year = (temp.empty() ? -1 : stoi(temp)); 
    // draft_round
    std::getline(is, temp, ',');
    player.draft_round = (temp.empty() ? -1 : stoi(temp)); 
    // draft_number
    std::getline(is, temp, ',');
    player.draft_number = (temp.empty() ? -1 : stoi(temp));
    return is; 
}