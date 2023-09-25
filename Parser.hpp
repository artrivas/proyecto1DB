#include <iostream>
#include <string>
#include "sequential/src/SequentialFile.hpp"
#include "exthash/src/ExtHashFile.hpp"
#include "avl/src/AVLFile.hpp"

using namespace std;

template<T>

class DataManager {
private:
    AVLFile<t,1> avlIndex;
    HashTable hashIndex;
    SequentialFile sequentialIndex;

public:
    DataManager(const string& avlFileName, const string& hashFileName, const string& sequentialFileName) {
        // Inicializar las instancias de los índices con los nombres de archivos proporcionados
        avlIndex.loadFromFile(avlFileName);
        hashIndex.loadFromFile(hashFileName);
        sequentialIndex.loadFromFile(sequentialFileName);
    }

    // Funciones para realizar operaciones en los índices
    void insertAVL(const string& key, const string& value) {
        avlIndex.insert(key, value);
    }

    string searchAVL(const string& key) {
        return avlIndex.search(key);
    }

    void insertHash(const string& key, const string& value) {
        hashIndex.insert(key, value);
    }

    string searchHash(const string& key) {
        return hashIndex.search(key);
    }

    void insertSequential(const string& key, const string& value) {
        sequentialIndex.insert(key, value);
    }

    string searchSequential(const string& key) {
        return sequentialIndex.search(key);
    }

    void saveIndexes() {
        // Guardar los índices en los archivos correspondientes
        avlIndex.saveToFile();
        hashIndex.saveToFile();
        sequentialIndex.saveToFile();
    }
};
