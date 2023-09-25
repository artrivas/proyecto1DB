
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "sequential/src/SequentialFile.hpp"
#include "exthash/src/ExtHashFile.hpp"
#include "avl/src/AVL.h"
#include "records/Record.h"
#include "utils/csvToBin.hpp"

using namespace std;

template<typename TR,typename TK>
class DataManager {
private:
    vector<AVLFile<TR, 50>> avl_indexes;
    vector<ExtHashFile<TR, TK>> hash_indexes;
    vector<SequentialFile<TR>> seq_indexes;

public:
    
    DataManager() {};
    
    void create_avl_index(string index_filename, string heap_filename) {
        //function<int(TK, TK)> compare_function = //;
        //bool pk = //;
        //int attrpos = //;
        //AVLFile<TK, attrpos> avl_index(index_filename, heap_filename, compare_function, pk);
        //avl_indexes.push_back(avl_index);
    }

    void create_hash_index(string filename, string index_field) {
        filename = "index/exthash/" + filename + "_data.bin";
        std::fstream file(filename, std::ios::binary | std::ios::app);
        file.close();
        ExtHashFile<TR, TK> hash_index(filename, index_field);
        hash_indexes.push_back(hash_index);
        csvToBin("assets/players2.csv", filename);

        std::fstream datafle(filename, std::ios::binary | std::ios::out | std::ios::in);
        datafile.seekg(0, std::ios::end);
        auto sz = datafile.tellg();

        datafile.seekg(0, std::ios::beg);
        for (int i = 0; i < sz/sizeof(TR); ++i) {
            TR record;
            datafile.read((char*) &record, sizeof(TR));
            hash_indexes[0].add(record);
        }
        datafile.close();
    }

    void create_seq_index(string datafile, string auxfile) {
        ExtHashFile<TR,TK> seq_index(datafile, auxfile);
        seq_indexes.push_back(seq_index);
    }

    void insert_avl_index(TR record) {
        if (avl_indexes[0].insert(record) == true) {
            cout << "Registro insertado correctamente\n";
        } else {
            cout << "Registro no insertado\n";
        }
    }

    void insert_hash_index(TR record) {
        if (hash_indexes[0].add(record)) {
            cout << "Registro insertado correctamente\n";
        } else {
            cout << "Registro no insertado\n";
        }
    }

    void insert_seq_index(TR record) {
        if (seq_indexes[0].add(record)) {
            cout << "Registro insertado correctamente\n";
        } else {
            cout << "Registro no insertado\n";
        }
    }

    void select_avl_index(TK key) {
        vector<Record> result = avl_indexes[0].find(key);
        if (result.empty()) {
            cout << "No se encontro registro con esa key\n";
            return;
        }
        for (auto& res: result) 
            res.print_vals();
    }
    
    void select_range_avl_index(TK begin_key, TK end_key) {
        vector<Record> result = avl_indexes[0].rangeSearch(begin_key, end_key);
        if (result.empty()) {
            cout << "No se encontraron registros con esa key\n";
            return;
        }
        for (auto& res: result) 
            res.print_vals();
    }

    void select_hash_index(TK key) {
        TR* result = hash_indexes[0].search(key);
        if (result == nullptr) {
            cout << "No se encontro registro con esa key\n";
            return;
        } 
        result->print();
    }
    
    void select_seq_index(TK key) {
        vector<TR> result = seq_indexes[0].search(key);
        if (result == nullptr) {
            cout << "No se encontro registro con esa key\n";
            return;
        } 
        for (auto& res: result) 
            res.print();
    }

    void select_range_seq_index(TK begin_key, TK end_key) {
        vector<TR> result = seq_indexes[0].rangeSearch(begin_key, end_key);
        if (result.empty()) {
            cout << "No se encontraron registros con esa key\n";
            return;
        }
        for (auto& res: result) 
            res.print();
    }

    void remove_avl_index(TK key) {
        if (avl_indexes[0].remove(key)) {
            cout << "Registro eliminado correctamente\n";
        } else {
            cout << "No se pudo eliminar\n";
        }
    }

    void remove_hash_index(TK key) {
        if (hash_indexes[0].remove(key)) {
            cout << "Registro eliminado correctamente\n";
        } else {
            cout << "No se pudo eliminar\n";
        }
    }

    void remove_seq_index(TK key) {
        if (seq_indexes[0].remove(key)) {
            cout << "Registro eliminado correctamente\n";
        } else {
            cout << "No se pudo eliminar\n";
        }
    }
};

