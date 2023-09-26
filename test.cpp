
#include <iostream>
#include "exthash/src/ExtHashFile.hpp"
#include "records/Player.hpp"
#include "utils/csvToBin.hpp"

using namespace std;

template <typename TypeKey>
void search(ExtHashFile<Player, int>& hash_index, TypeKey key) {
    Player* result = hash_index.search(key);
    if (result == nullptr) {
        cout << "No se encontro registro con esa key\n";
        return;
    } 
    std::cout << "\n";
    result->print();
    std::cout << "\n";
}

void add(ExtHashFile<Player, int>& hash_index, Player player) {
    bool result = hash_index.add(player);
    if (result) {
        cout << "Registro insertado correctamente\n";
    } else {
        cout << "No se pudo insertar porque ya esta indexado\n";
    }
}

template <typename TypeKey>
void remove(ExtHashFile<Player, int>& hash_index, TypeKey key) {
    bool result = hash_index.remove(key);
    if (result) {
        cout << "Registro eliminado correctamente\n";
    } else {
        cout << "No se pudo eliminar porque no existe o ya a sido eliminado\n";
    }
}

ExtHashFile<Player, int> create(string table_name, string index_field) {

    std::string datafile = "data/" + table_name + "_data.bin";
    std::fstream file(datafile, std::ios::binary | std::ios::app);
    file.close();

    ExtHashFile<Player, int> hash_index(table_name, index_field);        

    std::string bin_file = "bin/" + table_name + ".bin"; 
    std::fstream binfile(bin_file, std::ios::binary | std::ios::app);
    binfile.close();

    string file_path = "assets/players2.csv";  
    csvToBin<Player>(file_path, bin_file);

    std::fstream binFile(bin_file, std::ios::binary | std::ios::out | std::ios::in);
    binFile.seekg(0, std::ios::end);
    auto sz = binFile.tellg();

    binFile.seekg(0, std::ios::beg);
    for (int i = 0; i < sz/sizeof(Player); ++i) {
        Player record;
        binFile.read((char*) &record, sizeof(Player));
        hash_index.add(record);
    }
    binFile.close();
    
    return hash_index;
}

int main() {
    ExtHashFile<Player, int> exthash = create("player", "id");
    cout << exthash.count_records() << std::endl; // 548
    
    Player p1(48, "Juan", "Perez", "Guard", "6'5\"", 220, "1990-05 -15", "USA", "University of XYZ", 2010, 1, 1);
    add(exthash, p1);
    cout << exthash.count_records() << std::endl; // 549

    search<int>(exthash, 48);

    cout << "===================================================================\n";

    search<int>(exthash, 203937);
    remove<int>(exthash, 203937);
    search<int>(exthash, 203937);

    cout << "===================================================================\n";
    
    remove<int>(exthash, 48);
    search<int>(exthash, 48);
    
    return 0;
}

// g++ test.cpp .\records\Player.cpp .\exthash\utils\numToBin.cpp .\exthash\src\IndexEntry.cpp .\utils\getPageSize.cpp -o test