#include <iostream>

#include "./exthash/src/ExtHashFile.hpp"
#include "./exthash/src/Bucket.hpp"
#include "./records/Player.hpp"

int main() {
    ExtHashFile<Player, int> exthash("player", "id");
    std::cout << "===================================================================\n";
    std::cout << exthash.count_records() << std::endl; // 548
    return 0;
}