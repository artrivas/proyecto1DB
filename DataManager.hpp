#include <iostream>
#include <string>
#include <vector>
#include "sequential/src/SequentialFile.hpp"
#include "exthash/src/ExtHashFile.hpp"
#include "avl/src/AVLFile.hpp"

using namespace std;

template<typename TR,typename TK>

class DataManager {
private:
    vector<AVLFile<TR,50>> avlindices;
    vector<ExtHashFile<TR,int>> hasindices;
    vector<SequentialFile<TR>> sequentialindices;


public:
    DataManager(){};
    void createavl(string avlfilename, string heapfilename){
        AVLFile<TR,50> avlindice(avlfilename,heapfilename) ;
        avlindices.push_back(avlindice);
    }
    void createhashindice(string avlfilename,string id){
        ExtHashFile<TR,TK> hashindice(avlfilename,id) ;
        hasindices.push_back(hashindice);
    }

    void createsequential(string sequefilename,string aux){
        ExtHashFile<TR,TK> seqindice(sequefilename,aux) ;
        sequentialindices.push_back(seqindice);
    }

    void insertAVL(const TR record1) {
        avlIndex.insert(record1);
    }

    string searchAVL(TK key) {
        return avlIndex.search(key);
    }

    void insertHash(const TR registro) {
        hashIndex.add(registro);
    }

    string searchHash(TK key) {
        return hashIndex.search(key);
    }

    void insertSequential(const TR registro) {
        sequentialIndex.add(registro);
    }

    string searchSequential(TK key) {
        return sequentialIndex.search(key);
    }

    void saveIndexes() {
        avlIndex.saveToFile();
        hashIndex.saveToFile();
        sequentialIndex.saveToFile();
    }
};

