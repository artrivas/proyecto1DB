
#include <iostream>

#include "./exthash/src/ExtHashFile.hpp"
#include "./exthash/src/Bucket.hpp"
#include "./records/Player.hpp"

int main() {
    ExtHashFile<Player, int> exthash("players", "id");
    Player p1(48, "CCC", "Rpdasd", "Guard", "6'5\"", 220, "1990-05 -15", "USA", "University of XYZ", 2010, 1, 1);
    exthash.print();
    std::cout << "===================================================================\n";
    exthash.add(p1);
    exthash.print();
    return 0;
}