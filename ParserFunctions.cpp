//
// Created by artrivas on 9/26/23.
//
//
// Created by artrivas on 9/5/23.
//

#include "ParserFunctions.h"
using namespace std;

vector<Record> CsvToBin(const string & csvfile){
    ifstream readfile(csvfile);
    string inputline;
    vector<Record> ans;
    while(getline(readfile,inputline)){
        stringstream line(inputline);
        Record data;
        string temp;
        //PlayerId
        getline(line,temp,',');
        data.playerid = (temp.empty() ? -1:stoi(temp));
        //Fname
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.fname,temp.c_str());
        //Lname
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.lname,temp.c_str());
        //Position
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.position,temp.c_str());
        //Height
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.height,temp.c_str());
        //Weight
        getline(line,temp,',');
        data.weight = (temp.empty() ? -1:stoi(temp));

        //Birthday
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.birthday,temp.c_str());
        //Country
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.country,temp.c_str());

        //School
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.school,temp.c_str());

        //DraftYear
        getline(line,temp,',');
        data.draft_year = (temp.empty() ? -1:stoi(temp));

        //DraftRound
        getline(line,temp,',');
        data.draft_round = (temp.empty() ? -1:stoi(temp));

        //DraftNumber
        getline(line,temp,',');
        data.draft_number = (temp.empty() ? -1:stoi(temp));

        ans.push_back(data);
        inputline = "";
    }
    readfile.close();
    return ans;
}


void CsvToBinTwo(const string & csvfile,const string & binfile){
    ifstream readfile(csvfile);
    string inputline;
    while(getline(readfile,inputline)){
        stringstream line(inputline);
        Record data;
        string temp;
        //PlayerId
        getline(line,temp,',');
        data.playerid = (temp.empty() ? -1:stoi(temp));
        //Fname
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.fname,temp.c_str());
        //Lname
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.lname,temp.c_str());
        //Position
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.position,temp.c_str());
        //Height
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.height,temp.c_str());
        //Weight
        getline(line,temp,',');
        data.weight = (temp.empty() ? -1:stoi(temp));

        //Birthday
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.birthday,temp.c_str());
        //Country
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.country,temp.c_str());

        //School
        getline(line,temp,',');
        if(temp.empty())
            temp = "NULL";
        strcpy(data.school,temp.c_str());

        //DraftYear
        getline(line,temp,',');
        data.draft_year = (temp.empty() ? -1:stoi(temp));

        //DraftRound
        getline(line,temp,',');
        data.draft_round = (temp.empty() ? -1:stoi(temp));

        //DraftNumber
        getline(line,temp,',');
        data.draft_number = (temp.empty() ? -1:stoi(temp));

//        data.print_vals();
        ofstream writefile(binfile,ios::binary | ios::app);
        writefile.write((char*) &data,sizeof(Record));
        writefile.close();
        inputline = "";
    }
    readfile.close();
}
