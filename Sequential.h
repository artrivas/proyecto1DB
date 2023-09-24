//
// Created by sadaadsa on 9/3/2023.
//

#ifndef PROYECTO1_SEQ_H
#define PROYECTO1_SEQ_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <utility>
#include <algorithm>


using namespace std;

const char DATA_FILE = 'D';
const char AUX_FILE = 'A';

const int K = 70;

template <typename Record>
class SequentialFile{
private:

    string datafile_name;
    string auxfile_name;
    int numAuxEntries = 0;

    struct RecordBlock {
        long next;
        char next_file;
        Record record;

        RecordBlock() : RecordBlock(-1, DATA_FILE, Record()) {}
        RecordBlock(long _next, char _next_file, Record _record)
                : next(_next), next_file(_next_file), record(std::move(_record)) {}
    };



    void rebuild() {

        fstream auxfile(auxfile_name, ios::in | ios::binary);
        fstream datafile(datafile_name, ios::in | ios::binary);
        fstream newData("../tempdata.dat", ios::in | ios::out | ios::binary | ios::trunc);

        RecordBlock current;
        RecordBlock block;

        datafile.seekg(0, ios::beg);
        datafile.read((char*)&current, sizeof(RecordBlock));

        long pos_block = 0;

        block = current;
        block.next_file = DATA_FILE;
        block.next = (pos_block + 1) * sizeof(RecordBlock);

        newData.write((char*)&block, sizeof(RecordBlock));

        while (current.next != -1) {
            if (current.next_file == DATA_FILE) {
                datafile.seekg(current.next, ios::beg);
                datafile.read((char*)&block, sizeof(RecordBlock));
            } else if (current.next_file == AUX_FILE) {
                auxfile.seekg(current.next, ios::beg);
                auxfile.read((char*)&block, sizeof(RecordBlock));
            }

            current = block;
            pos_block++;

            block.next_file = DATA_FILE;
            if (block.next != -1) {
                block.next = (pos_block + 1) * sizeof(RecordBlock);
            }

            newData.write((char*)&block, sizeof(RecordBlock));
        }

        finalizeRebuild(datafile, auxfile, newData);
    }


    void finalizeRebuild(fstream &datafile, fstream &aux, fstream &newData) {
        datafile.close();
        aux.close();

        fstream auxNew(auxfile_name, ios::in | ios::out | ios::binary | ios::trunc);
        auxNew.close();

        std::remove(datafile_name.c_str());
        newData.close();
        std::rename("../tempdata.dat", datafile_name.c_str());
        numAuxEntries = 0;
    }


public:

    SequentialFile(string d, string a){
        datafile_name = std::move(d);
        auxfile_name = std::move(a);
        fstream datafile(datafile_name, ios::app);
        datafile.seekg(0, ios::end);
        if (datafile.tellg() == 0){
            RecordBlock header;
            header.next = -1;
            header.next_file = DATA_FILE;
            header.record = Record();
            datafile.write((char*)&header, sizeof(RecordBlock));
            fstream auxfile(auxfile_name, ios::app);
            auxfile.close();
        }
        datafile.close();
    }

    void BinToSequentialBlock(const string& binfile) {
    ifstream infile(binfile, ios::binary);
    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo binario." << endl;
        return;
    }
    Record r;
    while (infile.read((char*)&r, sizeof(Record))) {
        cout<<"Insertando:"<<endl;
        this->add(r);
    }

    infile.close();
    }

    void displayStructure() {
        fstream auxfile(auxfile_name, ios::in | ios::binary);
        fstream datafile(datafile_name, ios::in | ios::binary);

        RecordBlock block;
        char current_file = DATA_FILE;

        cout << "POS|" << "KEY|" << "NEXT" << endl;

        readBlock(datafile, 0, block);

        while (block.next != -1) {
            printBlockInfo(block, current_file, datafile, auxfile);

            if (block.next_file == AUX_FILE) {
                current_file = AUX_FILE;
                readBlock(auxfile, block.next / sizeof(RecordBlock), block);
            } else {
                current_file = DATA_FILE;
                readBlock(datafile, block.next / sizeof(RecordBlock), block);
            }
        }

        if (block.next == -1) {
            printBlockInfo(block, current_file, datafile, auxfile);
        }

        auxfile.close();
        datafile.close();
    }

    void printBlockInfo(RecordBlock &block, char current_file, fstream &datafile, fstream &auxfile) {
        int pos_block;
        if (current_file == DATA_FILE) {
            pos_block = ((long)datafile.tellg() - sizeof(RecordBlock)) / sizeof(RecordBlock);
        } else {
            pos_block = ((long)auxfile.tellg() - sizeof(RecordBlock)) / sizeof(RecordBlock);
        }
        cout << pos_block << current_file << " | ";
        cout << block.record.getPrimaryKey() << " | ";
        if (block.next != -1)
            cout << block.next / sizeof(RecordBlock) << block.next_file << endl;
        else
            cout << block.next << block.next_file << endl;
    }

    bool add(Record registro) {
        if (this->numAuxEntries == K) {
            rebuild();
        }

        fstream datafile(datafile_name, ios::in | ios::out | ios::binary);
        fstream auxfile(auxfile_name, ios::in | ios::out | ios::binary);

        if (addToDataIfGreaterThanLast(datafile, registro)) {
            datafile.close();
            auxfile.close();
            return true;
        }

        bool added = addToAux(datafile, auxfile, registro);

        datafile.close();
        auxfile.close();

        return added;
    }

    bool addToDataIfGreaterThanLast(fstream& datafile, const Record& registro) {
        RecordBlock last;

        datafile.seekg(-sizeof(RecordBlock), ios::end);
        datafile.read((char*)&last, sizeof(RecordBlock));
        if (last.record.getPrimaryKey() < registro.getPrimaryKey()) {
            RecordBlock new_block{ last.next, last.next_file, registro};

            datafile.seekp(0, ios::end);
            datafile.write((char*)&new_block, sizeof(RecordBlock));

            last.next = (long) datafile.tellp() - sizeof(RecordBlock);
            last.next_file = DATA_FILE;

            datafile.seekp(-2 * sizeof(RecordBlock), ios::cur);
            datafile.write((char*)&last, sizeof(RecordBlock));

            return true;
        }

        return false;
    }

    bool addToAux(fstream& datafile, fstream& auxfile, const Record& registro) {
        long current_pos = 0;
        char current_file = DATA_FILE;
        RecordBlock current;

        datafile.seekg(0, ios::beg);
        datafile.read((char*)&current, sizeof(RecordBlock));

        RecordBlock next;
        while (current.next != -1) {
            fstream& currentStream = (current.next_file == DATA_FILE) ? datafile : auxfile;
            currentStream.seekg(current.next, ios::beg);
            currentStream.read((char*)&next, sizeof(RecordBlock));

            if (next.record.getPrimaryKey() == registro.getPrimaryKey()) {
                return false;
            } else if (next.record.getPrimaryKey() > registro.getPrimaryKey()) {
                break;
            } else {
                current_pos = current.next;
                current_file = current.next_file;
                current = next;
            }
        }

        RecordBlock block{ current.next, current.next_file, registro };

        auxfile.seekp(0, ios::end);
        long pos = auxfile.tellp();
        auxfile.write((char*)&block, sizeof(RecordBlock));

        current.next = pos;
        current.next_file = AUX_FILE;

        fstream& targetStream = (current_file == DATA_FILE) ? datafile : auxfile;
        targetStream.seekp(current_pos, ios::beg);
        targetStream.write((char*)&current, sizeof(RecordBlock));

        numAuxEntries++;
        return true;
    }


    template<typename T>
    bool remove(T key){
        fstream auxfile(auxfile_name, ios::in | ios::out | ios::binary);
        fstream datafile(datafile_name, ios::in | ios::out | ios::binary);
        long current_pos = 0;
        RecordBlock current, next;

        datafile.seekg(0, ios::beg);
        datafile.read((char*)&current, sizeof(RecordBlock));

        bool keyFound = false;
        char current_file = DATA_FILE;

        while (current.next != -1) {
            fstream &activeFile = (current.next_file == DATA_FILE) ? datafile : auxfile;
            activeFile.seekg(current.next, ios::beg);
            activeFile.read((char*)&next, sizeof(RecordBlock));
            if (next.record.getPrimaryKey() == key) {
                keyFound = true;
                long temp_pos = next.next;
                next.next = -2;
                activeFile.seekg(current.next, ios::beg);
                activeFile.write((char*)&next, sizeof(RecordBlock));
                current.next = temp_pos;
                current.next_file = next.next_file;
                if (current_file == DATA_FILE) {
                    datafile.seekg(current_pos, ios::beg);
                    datafile.write((char*)&current, sizeof(RecordBlock));
                } else {
                    auxfile.seekg(current_pos, ios::beg);
                    auxfile.write((char*)&current, sizeof(RecordBlock));
                }
                break;
            } else if (next.record.getPrimaryKey() > key) {
                break;
            }

            current_pos = current.next;
            current_file = current.next_file;
            current = next;
        }

        datafile.close();
        auxfile.close();

        if (keyFound) {
            return true;
        }
        return false;
    }

    template<typename T>
    vector<Record> search(T key) {
        vector<Record> result;

        if (binarySearchInDatafile(key, result)) {
            return result;
        }

        linearSearchInAuxfile(key, result);

        return result;
    }

    template<typename T>
    bool binarySearchInDatafile(T key, vector<Record>& result) {
        fstream datafile(datafile_name, ios::in | ios::binary);
        RecordBlock current;

        long long fileSize = datafile.tellg();
        int low = 0, high = (fileSize / sizeof(RecordBlock)) - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            datafile.seekg(mid * sizeof(RecordBlock), ios::beg);
            datafile.read((char*)&current, sizeof(RecordBlock));

            if (current.record.getPrimaryKey() == key && current.next != -2) {
                result.push_back(current.record);
                datafile.close();
                return true;
            }

            if (current.record.getPrimaryKey() < key) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        datafile.close();
        return false;
    }

    template<typename T>
    void linearSearchInAuxfile(T key, vector<Record>& result) {
        fstream auxfile(auxfile_name, ios::in | ios::binary);
        RecordBlock current;

        while (auxfile.read((char*)(&current), sizeof(RecordBlock))) {
            if (current.record.getPrimaryKey() == key && current.next != -2) {
                result.push_back(current.record);
            }
        }

        auxfile.close();
    }

    template<typename T>
    vector<Record> rangeSearch(T begin_key, T end_key) {
        vector<Record> result;

        fstream datafile(datafile_name, ios::in | ios::binary);
        searchInFile(datafile, begin_key, end_key, result);
        datafile.close();

        fstream auxfile(auxfile_name, ios::in | ios::binary);
        searchAuxFile(auxfile, begin_key, end_key, result);
        auxfile.close();

        return result;
    }

    template<typename T>
    void searchInFile(fstream &datafile, T begin_key, T end_key, vector<Record> &res) {
        RecordBlock current;
        datafile.seekg(0, ios::end);
        long long n = datafile.tellg() / sizeof(RecordBlock);
        int low = 0, high = n - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            readBlock(datafile, mid, current);
            if (current.record.getPrimaryKey() < begin_key) {
                low = mid + 1;
            }
            else if (current.record.getPrimaryKey() > end_key) {
                high = mid - 1;
            }
            else {
                gatherRecordsInRange(datafile, mid, begin_key, end_key, res);
                low = mid + 1;
                high = mid - 1;
            }
        }
    }

    void readBlock(fstream &datafile, int position, RecordBlock &block) {
        datafile.seekg(position * sizeof(RecordBlock), ios::beg);
        datafile.read((char*)&block, sizeof(RecordBlock));
    }

    template<typename T>
    void gatherRecordsInRange(fstream &datafile, int mid, T begin_key, T end_key, vector<Record> &res) {
        RecordBlock current;
        int i = mid;
        readBlock(datafile, i, current);
        while (current.record.getPrimaryKey() <= end_key) {
            if (current.next != -2 && current.record.getPrimaryKey() >= begin_key) {
                res.push_back(current.record);
            }
            i++;
            readBlock(datafile, i, current);
        }

        i = mid - 1;
        readBlock(datafile, i, current);
        while (i >= 0 && current.record.getPrimaryKey() >= begin_key) {
            if (current.next != -2 && current.record.getPrimaryKey() <= end_key) {
                res.push_back(current.record);
            }
            i--;
            if (i >= 0) {
                readBlock(datafile, i, current);
            }
        }
    }


    template<typename T>
    void searchAuxFile(fstream &auxfile, T begin_key, T end_key, vector<Record> &res) {
        RecordBlock current;
        auxfile.seekg(0, ios::beg);
        while (auxfile.read((char*)(&current), sizeof(RecordBlock))) {
            if (current.record.getPrimaryKey() >= begin_key && current.record.getPrimaryKey() <= end_key && current.next != -2) {
                res.push_back(current.record);
            }
        }
    }

};





#endif //PROYECTO1_SEQ_H
