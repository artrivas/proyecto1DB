#include <iostream>
#include <vector>
#include <cstring>
#include "sequential/src/SequentialFile.hpp"
#include "records/Record.h"

#include "ParserFunctions.h"
#include <cassert>

using namespace std;


void testSearch(SequentialFile<Record>& sqf) {
    // Test 1: Trae Young
    vector<Record> result1 = sqf.search(1629027);
    assert(result1.size() == 1);
    assert(result1[0].playerid == 1629027);
    assert(strcmp(result1[0].fname, "Trae") == 0);

    // Test 2: Justise Winslow
    vector<Record> result2 = sqf.search(1626159);
    assert(result2.size() == 1);
    assert(result2[0].playerid == 1626159);
    assert(strcmp(result2[0].fname, "Justise") == 0);

    // Test 3: Thaddeus Young
    vector<Record> result3 = sqf.search(201152);
    assert(result3.size() == 1);
    assert(result3[0].playerid == 201152);
    assert(strcmp(result3[0].fname, "Thaddeus") == 0);

    // Test 4: Omer Yurtseven
    vector<Record> result4 = sqf.search(1630209);
    assert(result4.size() == 1);
    assert(result4[0].playerid == 1630209);
    assert(strcmp(result4[0].fname, "Omer") == 0);

    // Test 5: Gabe York
    vector<Record> result5 = sqf.search(1628221);
    assert(result5.size() == 1);
    assert(result5[0].playerid == 1628221);
    assert(strcmp(result5[0].fname, "Gabe") == 0);

    std::cout << "Search tests passed!" << std::endl;
}


void testRangeSearch(SequentialFile<Record>& sqf) {
    // Test 1
    vector<Record> result1 = sqf.rangeSearch(201988, 202699);
    assert(result1.size() == 20);

    // Test 2
    vector<Record> result2 = sqf.rangeSearch(1629622, 1629643);
    assert(result2.size() == 19);

    // Test 3
    vector<Record> result3 = sqf.rangeSearch(1630600, 1630639);
    assert(result3.size() == 9);

    // Test 4
    vector<Record> result4 = sqf.rangeSearch(1631104, 1631165);
    assert(result4.size() == 24);

    // Test 5
    vector<Record> result5 = sqf.rangeSearch(1631213, 1631466);
    assert(result5.size() == 23);

    std::cout << "Range search tests passed!" << std::endl;
}


void testRemove(SequentialFile<Record>& sqf) {
    // Test 1
    assert(sqf.remove(1629027) == true);
    assert(sqf.search(1629027).size() == 0);

    // Test 2
    assert(sqf.remove(1626159) == true);
    assert(sqf.search(1626159).size() == 0);

    // Test 3
    assert(sqf.remove(201152) == true);
    assert(sqf.search(201152).size() == 0);

    // Test 4
    assert(sqf.remove(1630209) == true);
    assert(sqf.search(1630209).size() == 0);

    // Test 5
    assert(sqf.remove(1628221) == true);
    assert(sqf.search(1628221).size() == 0);

    std::cout << "Remove tests passed!" << std::endl;
}


void testAdd(SequentialFile<Record>& sqf) {
    Record record;

    // Test 1
    record.playerid = 4000001;
    strcpy(record.fname, "TestPlayer1");
    assert(sqf.add(record) == true);
    assert(sqf.search(4000001).size() == 1);

    // Test 2
    record.playerid = 4000002;
    strcpy(record.fname, "TestPlayer2");
    assert(sqf.add(record) == true);
    assert(sqf.search(4000002).size() == 1);

    // Test 3
    record.playerid = 4000003;
    strcpy(record.fname, "TestPlayer3");
    assert(sqf.add(record) == true);
    assert(sqf.search(4000003).size() == 1);

    // Test 4
    record.playerid = 4000004;
    strcpy(record.fname, "TestPlayer4");
    assert(sqf.add(record) == true);
    assert(sqf.search(4000004).size() == 1);

    // Test 5
    record.playerid = 4000005;
    strcpy(record.fname, "TestPlayer5");
    assert(sqf.add(record) == true);
    assert(sqf.search(4000005).size() == 1);

    std::cout << "Add tests passed!" << std::endl;
}



int main() {
    CsvToBinTwo("players.csv", "players.bin");
    SequentialFile<Record> sqf("datafile.bin", "auxiliar_file.bin");
    sqf.BinToSequentialBlock("players.bin");
    testSearch(sqf);
    testRangeSearch(sqf);
    testRemove(sqf);
    testAdd(sqf);
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}

