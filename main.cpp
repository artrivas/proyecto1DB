
#include "./exthash/src/ExtHashFile.hpp"
#include "./exthash/src/Bucket.hpp"
#include "./records/Player.hpp"

//#include <iostream>
//#include "./utils/getPageSize.hpp"
//using namespace std;
#include "./exthash/utils/hasher.hpp"
#include "./exthash/utils/numToBin.hpp"

int maindd() {
    return 0;

}

int maiddn() {
    int d = 6;
    int num1 = hasher(86) % int(std::pow(2, d));
    int num2 = hasher(48) % int(std::pow(2, d));
    int num3 = hasher(480) % int(std::pow(2, d));
    
    /*
    int i = 1;
    int num;
    std::string binary;
    while (true) {
        num = hasher(i) % int(std::pow(2, d));
        binary = numToBin(num, d);
        if (i == 48) {
            i++;
            continue;
        }
        if (binary.substr(binary.length() - 5) == "00110") {
            std::cout << i << std::endl;
            std::cout << binary << std::endl;
            break;
        }
        //std::cout << i << ": " << binary << std::endl;
        i++;
    }
    */
    
   std::string binary1 = numToBin(num1, d);
   std::string binary2 = numToBin(num2, d);
   std::string binary3 = numToBin(num3, d);
   std::cout << binary1 << std::endl;
   std::cout << binary2 << std::endl;
   std::cout << binary3 << std::endl;
   
    
    return 0;
}

int main() {   
    ExtHashFile<Player, int> e("players", "id");
    e.print();
    //std::cout << "================================================\n";
    Player p1(480, "AAA", "Rpdasd", "Guard", "6'5\"", 220, "1990-05-15", "USA", "University of XYZ", 2010, 1, 1);    
    //Player p2(86, "BBB", "Rpdasd", "Guard", "6'5\"", 220, "1990-05-15", "USA", "University of XYZ", 2010, 1, 1);    
    //Player p3(48, "CCC", "Rpdasd", "Guard", "6'5\"", 220, "1990-05-15", "USA", "University of XYZ", 2010, 1, 1);    
    e.add(p1);
    //e.add(p2);
    //e.add(p3);
    std::cout << "================================================\n";
    //auto ptr = e.search(480);
    //e.search(480);
    //e.add(p1);
    //e.remove(86);
    e.print();

    //if (ptr != nullptr)
    //    ptr->print();
    //else std::cout << "es un puntero\n";
    //e.print();
    //auto r = e.search(480);
    //Player* p = &r;
    //r.print();
    //p->print();
    //e.print();
    //std::cout << "================================================\n";
    //e.add(p1);
    //std::cout << "================================================\n";
    //e.print();
    //std::cout << "================================================\n";
    //e.add(p2);
    //std::cout << "================================================\n";


    return 0;
}

int madin() {
    ExtHashFile<Player, int> e("players", "id"); 
    e.print();
    std::cout << "================================================\n";
    auto ptr = e.search(48);

    if (ptr != nullptr) {
        (*ptr).print();
    } else {
        std::cout << "Es un puntero\n";
    }
    
    //ptr.print();
    return 0;
}