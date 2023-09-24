
#include <iostream>

#include "./exthash/src/ExtHashFile.hpp"
#include "./exthash/src/Bucket.hpp"
#include "./records/Player.hpp"

int main() {
    ExtHashFile<Player, int> exthash("players", "id");
    exthash.print();
    return 0;
}