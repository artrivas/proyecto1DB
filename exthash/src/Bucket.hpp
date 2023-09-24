
#pragma once

#include <fstream>
#include <vector>
#include "./../utils/hasher.hpp"
#include "./../utils/numToBin.hpp"

struct MetadataB {
    int capacity;
    int count;
    int local_depth;  
    int head_ptr;
    int empty_ptr;
};

template <typename TR>
struct Bucket {
    MetadataB metadata;
    int* next_del;
    TR* records;

    Bucket(int block_factor);
    ~Bucket();

    void write(std::fstream& file);
    void read(std::fstream& file);

    Bucket<TR> split();

    void add(std::fstream &dataFile, int pos_bucket, TR record); // also write
    void add(TR record); 
    void remove(int pos); 
    
    void print();
    bool is_full();
    bool is_empty();
    int size_of();

    void set_next_empty();
    bool evaluate_split();
    char get_common_bit();
};

// Implementation of Bucket

template <typename TR>
Bucket<TR>::Bucket(int block_factor) {
    metadata.capacity = block_factor;
    metadata.count = 0; // number of real records
    metadata.local_depth = 0;
    metadata.head_ptr = -1;
    metadata.empty_ptr = 1;
    next_del = new int[metadata.capacity]; 
    records = new TR[metadata.capacity]; 
    for (int i = 0; i < metadata.capacity; ++i)
        next_del[i] = -2; // empty
}

template <typename TR>
Bucket<TR>::~Bucket() {
    delete[] next_del;
    delete[] records;
}

template <typename TR>
void Bucket<TR>::write(std::fstream& file) {
    file.write((char*) &metadata, sizeof(metadata));
    for (int i = 0; i < metadata.capacity; ++i)
        file.write((char*) &next_del[i], sizeof(int));
    for (int i = 0; i < metadata.capacity; ++i) 
        file.write((char*) &records[i], sizeof(TR));        
}

template <typename TR>
void Bucket<TR>::read(std::fstream& file) {
    file.read((char*) &metadata, sizeof(metadata));
    for (int i = 0; i < metadata.capacity; ++i)
        file.read((char*) &next_del[i], sizeof(int));
    for (int i = 0; i < metadata.capacity; ++i) 
        file.read((char*) &records[i], sizeof(TR));        
}

template <typename TR>
Bucket<TR> Bucket<TR>::split() {
    Bucket<TR> new_bucket(metadata.capacity);
    new_bucket.metadata.local_depth = metadata.local_depth; // important
    std::vector<std::string> binaries;
    
    for (int i = 0; i < metadata.capacity; i++) {
        int num = hasher(records[i].get_key()) % int(std::pow(2, metadata.local_depth));
        std::string binary = numToBin(num, metadata.local_depth);
        binaries.push_back(binary);
    }

    for (int i = 0; i < binaries.size(); ++i) {
        if (binaries[i][0] == '1') { // msb in reverse binary
            new_bucket.add(records[i]);
            remove(i);
        }
    }
    return new_bucket;
}

template <typename TR>
void Bucket<TR>::add(std::fstream &dataFile, int pos_bucket, TR record) {
    int pos = (pos_bucket * size_of()) + (3 * sizeof(int));
    if (is_full()) {
        std::cout << "The bucket is full\n";
        return;
    }

    metadata.count++;
    if (((metadata.empty_ptr < metadata.head_ptr) || (metadata.head_ptr == -1)) && (metadata.empty_ptr != -1)) {
        int pos_empty = metadata.empty_ptr - 1; 
        records[pos_empty].print(); 
        records[pos_empty] = record;
        records[pos_empty].print(); 
        next_del[pos_empty] = 0;

        dataFile.seekp(pos + (sizeof(int) * (5 + pos_empty)), std::ios::beg);
        dataFile.write((char*) &next_del[pos_empty], sizeof(int));
        dataFile.seekp(pos + (sizeof(int) * (5 + metadata.capacity)) + (sizeof(TR)*pos_empty) , std::ios::beg);
        dataFile.write((char*) &records[pos_empty], sizeof(TR));
        dataFile.seekp(pos + sizeof(int), std::ios::beg);
        dataFile.write((char*) &metadata.count, sizeof(int));       
    } else {
        int aux_pos = metadata.head_ptr - 1;    
        metadata.head_ptr = next_del[metadata.head_ptr - 1];
        records[aux_pos] = record;
        next_del[aux_pos] = 0;

        dataFile.seekp(pos + sizeof(int)*(5 + aux_pos), std::ios::beg);
        dataFile.write((char*) &next_del[aux_pos], sizeof(int));
        dataFile.seekp(pos + (sizeof(int)*(5 + metadata.capacity)) + (sizeof(TR)*aux_pos) , std::ios::beg);
        dataFile.write((char*) &records[aux_pos], sizeof(TR));
        dataFile.seekp(pos + sizeof(int), std::ios::beg);
        dataFile.write((char*) &metadata.count, sizeof(int));
        dataFile.seekp(pos + sizeof(int)*3, std::ios::beg);
        dataFile.write((char*) &metadata.head_ptr, sizeof(int));
    }

    set_next_empty();
    dataFile.seekp(pos + sizeof(int)*4, std::ios::beg);
    dataFile.write((char*) &metadata.empty_ptr, sizeof(int));
}

template <typename TR>
void Bucket<TR>::add(TR record) {
    if (is_full()) {
        std::cout << "The bucket is full\n";
        return;
    }
    metadata.count++;
    if ((metadata.empty_ptr < metadata.head_ptr) || (metadata.head_ptr == -1)) {
        int pos_empty = metadata.empty_ptr - 1;
        records[pos_empty] = record;
        next_del[pos_empty] = 0;
    } else {
        int aux_pos = metadata.head_ptr - 1;    
        metadata.head_ptr = next_del[metadata.head_ptr - 1];
        records[aux_pos] = record;
        next_del[aux_pos] = 0;
    }
    set_next_empty();
}

template <typename TR>
void Bucket<TR>::remove(int pos) { 
    if (is_empty()) {
        std::cout << "The bucket is empty\n";
        return;
    }
    if (next_del[pos] == 0) {
        int aux_ptr = metadata.head_ptr;
        metadata.head_ptr = pos + 1; // position starts from one
        next_del[pos] = aux_ptr; 
        metadata.count--;
    } else {
        std::cout << "The record for this position has already been deleted or is empty\n";
    }
}

template <typename TR>
void Bucket<TR>::print() {
    std::cout << "\ncapacity: " << metadata.capacity << std::endl;
    std::cout << "count: " << metadata.count << std::endl;
    std::cout << "local_depth: " << metadata.local_depth << std::endl;
    std::cout << "head_ptr: " << metadata.head_ptr << std::endl;
    std::cout << "empty_ptr: " << metadata.empty_ptr << std::endl;
    std::cout << "next_del: | ";
    for (int i = 0; i < metadata.capacity; ++i) 
        std::cout << next_del[i] << " | ";
    std::cout << "\n";
    for (int i = 0; i < metadata.capacity; ++i) {
        std::cout << "\n";
        records[i].print();
    }
    std::cout << "\n------------------------------\n";
}

template <typename TR>
bool Bucket<TR>::is_full() {
    return metadata.count == metadata.capacity;    
}

template <typename TR>
bool Bucket<TR>::is_empty() {
    return metadata.count == 0;    
}
template <typename TR>
int Bucket<TR>::size_of() {
    return (sizeof(int)*(5 + metadata.capacity)) + (sizeof(TR)*metadata.capacity);
}

template <typename TR>
void Bucket<TR>::set_next_empty() {
    if (metadata.empty_ptr == -1) {
        return;
    }
    int pos_empty = metadata.empty_ptr - 1;
    for (int i = pos_empty + 1 ; i < metadata.capacity; ++i) {
        if (next_del[i] == -2) {
            metadata.empty_ptr = i + 1;
            return;
        }
    }
    metadata.empty_ptr = -1;
}

template <typename TR>
bool Bucket<TR>::evaluate_split() {
    metadata.local_depth++;
    char bit = get_common_bit();
    metadata.local_depth--;
    return bit == ' ';
}

template <typename TR>
char Bucket<TR>::get_common_bit() {
    std::vector<std::string> binaries;
    for (int i = 0; i < metadata.capacity; i++) {
        if (next_del[i] != -2) { // not empty
            int num = hasher(records[i].get_key()) % int(std::pow(2, metadata.local_depth));
            std::string binary = numToBin(num, metadata.local_depth);
            binaries.push_back(binary); // reverse
        }
    }
    char bit = binaries[0][0];
    for (int i = 1; i < binaries.size(); i++) {
        char b = binaries[i][0];
        if (bit != b) return ' ';
    }
    return bit;
}