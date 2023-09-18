
#include <bitset>
#include "numToBin.hpp"

std::string numToBin(int num, int n) {
    return std::bitset<32>(num).to_string().substr(32 - n, n);
}