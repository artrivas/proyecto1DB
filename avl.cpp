//4096 bytes - Sector Capacity
#include<iostream>
#include <string.h>
#include<fstream>
#include<sstream>
using namespace std;

struct Record{
    long playerid;
    char fname[20];
    char lname[20];
    char position[20];
    char height[5];
    int weight;
    char birthday[11];
    char country[15];
    char school[25];
    int draft_year;
    int draft_round;    
    int draft_number;    
    void print_vals(){
        cout << "Player Id: "<<playerid << '\n';
        cout << "Fist Name: "<<fname << '\n';
        cout << "Last Name: "<<lname << '\n';
        cout << "Position: "<<position << '\n';
        cout << "Height: "<<height << '\n';
        cout << "Weight: "<<weight << '\n';
        cout << "Birthday: "<<birthday << '\n';
        cout << "Country: "<<country << '\n';
        cout << "School: "<<school << '\n';
        cout << "Draft Year: "<<draft_year << '\n';
        cout << "Draft Round: " << draft_round << '\n';
        cout << "Draft Number: " << draft_number << '\n';
    }
};

int main(){
    ifstream file("players.csv");
    string inputline = "";
    getline(file,inputline);

    stringstream line(inputline);
    Record data;
    string temp;
    //PlayerId
    getline(line,temp,',');
    data.playerid = stoi(temp.c_str());
    //Fname
    getline(line,temp,',');
    strcpy(data.fname,temp.c_str());
    //Lname
    getline(line,temp,',');
    strcpy(data.lname,temp.c_str());
    //Position
    getline(line,temp,',');
    strcpy(data.position,temp.c_str());
    //Height
    getline(line,temp,',');
    strcpy(data.height,temp.c_str());
    //Weight
    getline(line,temp,',');
    data.weight = stoi(temp.c_str());

    //Birthday
    getline(line,temp,',');
    strcpy(data.birthday,temp.c_str());
    //Country
    getline(line,temp,',');
    strcpy(data.country,temp.c_str());

    //School
    getline(line,temp,',');
    strcpy(data.school,temp.c_str());

    //DraftYear
    getline(line,temp,',');
    data.draft_year = stoi(temp.c_str());

    //DraftRound
    getline(line,temp,',');
    data.draft_round = stoi(temp.c_str());

    //DraftNumber
    getline(line,temp,',');
    data.draft_number = stoi(temp.c_str());

    file.close();
    data.print_vals();
    return 0;
}
