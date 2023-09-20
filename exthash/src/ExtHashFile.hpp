
#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "Bucket.hpp"
#include "IndexEntry.hpp"
#include "./../../utils/getPageSize.hpp"
#include "./../utils/numToBin.hpp"
#include "./../utils/hasher.hpp"

const std::string datapath = "./data/";
const std::string indexpath = "./index/exthash/";

struct MetadataEHF {
    int count;
    int global_depth;
    int block_factor;
};

template <typename TR, typename TK>
class ExtHashFile {
private:
    MetadataEHF metadata;
    std::string datafile;
    std::string indexfile;
    std::string datafilefl;
    std::string indexfilefl;

    int count_index{};
    
    void find_node_pos(std::fstream& file, std::string& binary, int& pos_node, int& prev_pos_node, int& depth);
    TR* find_record(TK key, int& pos_bucket, TR& record, int& pos_record, bool& found);
    void add_record_rehashing(std::fstream& indexFile, std::fstream& dataFile, TR record); 
    bool is_empty(std::fstream& file);
    int size(std::fstream& file);
    int capacity(std::fstream& file);

public:
    ExtHashFile(std::string filename, std::string index_field);
    TR* search(TK key);
    bool add(TR record);
    bool remove(TK key);
    void print();

    int get_pos_add_bucket(std::fstream& file); 
    int get_pos_add_node(std::fstream& file); 
    std::vector<int> get_path_traversal(std::fstream& file, int key, std::string& binary);

    void remove_bucket(std::fstream& file, int pos_bucket);
    void remove_node(std::fstream& file, std::vector<int> nodes_to_delete);
};

// Implementation of ExtHashFile

template <typename TR, typename TK>
ExtHashFile<TR, TK>::ExtHashFile(std::string filename, std::string index_field) {
    datafile = datapath + filename + "_data.bin";
    datafilefl = datapath + filename + "_data_fl.bin";
    indexfile = indexpath + filename + "_index_eh_" + index_field + ".bin";
    indexfilefl = indexpath + filename + "_index_eh_" + index_field + "_fl.bin";

    std::fstream inFile(datafile, std::ios::binary | std::ios::in);

    if (!inFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 
    
    if (!is_empty(inFile)) {
        inFile.read((char*) &metadata, sizeof(metadata));
        inFile.close();
    }
    else {
        inFile.close();
        std::fstream outFile(datafile, std::ios::binary | std::ios::out);
        if (!outFile.is_open()) 
            throw std::runtime_error("Cannot open datafile");
        
        metadata.global_depth = 1; // default
        
        // 5 metadata | records and next_del 
        // metadata.block_factor = (getPageSize() - sizeof(int)*5) / (sizeof(TR)+sizeof(int));
        metadata.block_factor = 2;

        metadata.count = 2; // default

        outFile.write((char*) &metadata, sizeof(metadata));

        for (int i = 0; i < metadata.count; ++i) {
            Bucket<TR> bucket(metadata.block_factor);
            bucket.metadata.local_depth++;
            bucket.write(outFile);
        }
        outFile.close();

        // create data_fl file
        std::fstream dataFileFL(datafilefl, std::ios::binary | std::ios::out | std::ios::app);
        if (!dataFileFL.is_open())
            throw std::runtime_error("Cannot open datafl_file"); 
        
        int header = -1;
        dataFileFL.write((char*) &header, sizeof(header));
        for (int i = 0; i < metadata.count; ++i) {
            int next_d{};
            dataFileFL.write((char*) &next_d, sizeof(next_d));
        }
        dataFileFL.close();

        // create index file
        std::fstream indexFile(indexfile, std::ios::binary | std::ios::out | std::ios::app);
        if (!indexFile.is_open())
            throw std::runtime_error("Cannot open indexfile"); 
        
        IndexEntry root(1, 2, -1);
        count_index++;
        IndexEntry left(-1, -1, 0);
        count_index++;
        IndexEntry right(-1, -1, 1);
        count_index++;
        indexFile.write((char*) &count_index, sizeof(count_index));
        indexFile.write((char*) &root, sizeof(root));
        indexFile.write((char*) &left, sizeof(left));
        indexFile.write((char*) &right, sizeof(right));
        indexFile.close();

        // create index_fl file
        std::fstream indexFileFL(indexfilefl, std::ios::binary | std::ios::out | std::ios::app);
        if (!indexFileFL.is_open())
            throw std::runtime_error("Cannot open indexfl_file"); 
        
        header = -1;
        indexFileFL.write((char*) &header, sizeof(header));
        for (int i = 0; i < count_index; ++i) {
            int next_d{};
            indexFileFL.write((char*) &next_d, sizeof(next_d));
        }
        indexFileFL.close();
    }
}

template <typename TR, typename TK>
TR* ExtHashFile<TR, TK>::search(TK key) {
    int pos_bucket{};
    static TR record;
    int pos_record{};
    bool found {false};
    find_record(key, pos_bucket, record, pos_record, found);
    if (found) 
        return &record;
    return nullptr;
}

template <typename TR, typename TK>
bool ExtHashFile<TR, TK>::add(TR record) {
    std::fstream indexFile(indexfile, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFile.is_open())
        throw std::runtime_error("Cannot open indexfile"); 
    
    std::fstream dataFile(datafile, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 
    
    std::fstream indexFileFL(indexfilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFileFL.is_open())
        throw std::runtime_error("Cannot open index_fl file"); 
    
    std::fstream dataFileFL(datafilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFileFL.is_open())
        throw std::runtime_error("Cannot open data_fl file"); 
    
    size_t num = hasher(record.get_key()) % int(std::pow(2, metadata.global_depth));
    std::string binary = numToBin(num, metadata.global_depth);
    std::reverse(binary.begin(), binary.end());

    if (is_empty(indexFile)) {
        std::cout << "There are no records in the datafile\n";
        return false;
    }

    IndexEntry node;
    int prev_pos_node{};
    int pos_node{}; 
    indexFile.read((char*) &count_index, sizeof(count_index));
    int depth{};

    find_node_pos(indexFile, binary, pos_node, prev_pos_node, depth);

    if (pos_node != -1) {
        indexFile.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));
        Bucket<TR> bucket(metadata.block_factor);
        dataFile.seekg((node.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
        bucket.read(dataFile);

        if (bucket.is_full()) {
            std::vector<IndexEntry> nodes;
            std::string bit_sequence = "";
            while (!bucket.evaluate_split()) { 
                if (metadata.global_depth == bucket.metadata.local_depth) 
                    metadata.global_depth++;
                bucket.metadata.local_depth++;
                IndexEntry new_node;
                nodes.emplace_back(new_node);
                bit_sequence += bucket.get_common_bit(); 
            };
            if (metadata.global_depth == bucket.metadata.local_depth) 
                metadata.global_depth++;
            bucket.metadata.local_depth++;

            IndexEntry n_left;
            IndexEntry n_right;

            Bucket<TR> new_bucket_right = bucket.split(); 

            if (bit_sequence.empty()) {
                node.left = get_pos_add_node(indexFileFL); // count_index;
                count_index++;
                node.right = get_pos_add_node(indexFileFL); // count_index;
                count_index++;
                n_left.pos_bucket = node.pos_bucket;
                n_right.pos_bucket = get_pos_add_bucket(dataFileFL); // metadata.count;
                metadata.count++;
                node.pos_bucket = -1;
            } else {
                if (bit_sequence[0] == '0') 
                    node.left = get_pos_add_node(indexFileFL); // count_index;
                else 
                    node.right = get_pos_add_node(indexFileFL); // count_index;
                count_index++;

                int i;
                for (i = 0; i < nodes.size()-1; ++i) {
                    if (bit_sequence[i + 1] == '0') 
                        nodes[i].left = get_pos_add_node(indexFileFL); // count_index;
                    else 
                        nodes[i].right = get_pos_add_node(indexFileFL); // count_index;
                    count_index++;
                }
                nodes[i].left = get_pos_add_node(indexFileFL); // count_index;
                count_index++;
                nodes[i].right = get_pos_add_node(indexFileFL); // count_index;
                count_index++;
                n_left.pos_bucket = node.pos_bucket;
                n_right.pos_bucket = get_pos_add_bucket(dataFileFL); // metadata.count;
                metadata.count++;
                node.pos_bucket = -1;
            }

            // write bucket count and global_depth update
            dataFile.seekp(0, std::ios::beg);
            dataFile.write((char*) &metadata.count, sizeof(int)); 
            dataFile.write((char*) &metadata.global_depth, sizeof(int)); 

            // write node count update
            indexFile.seekp(0, std::ios::beg);
            indexFile.write((char*) &count_index, sizeof(int)); 

            // update bucket
            dataFile.seekp((n_left.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
            dataFile.write((char*) &bucket.metadata, sizeof(bucket.metadata));
            for (int i = 0; i < bucket.metadata.capacity; ++i) {
                dataFile.write((char*) &bucket.next_del[i], sizeof(int));   
            }
            
            // write new bucket
            dataFile.seekp(0, std::ios::end);
            new_bucket_right.write(dataFile);  
            
            // write node updated
            indexFile.seekp(pos_node*sizeof(IndexEntry) + sizeof(count_index), std::ios::beg);
            indexFile.write((char*) &node, sizeof(IndexEntry));
            
            // write news nodes
            indexFile.seekp(0, std::ios::end);
            for (int i = 0; i < nodes.size(); ++i) {
                indexFile.write((char*) &nodes[i], sizeof(IndexEntry));
            }
            indexFile.write((char*) &n_left, sizeof(IndexEntry)); 
            indexFile.write((char*) &n_right, sizeof(IndexEntry)); 

            add_record_rehashing(indexFile, dataFile, record); 
        }
        else {
            bucket.add(dataFile, node.pos_bucket, record);
        }
    } else {
        indexFile.seekg((prev_pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));

        Bucket<TR> new_buck(metadata.block_factor);
        new_buck.metadata.local_depth = depth + 1;
        IndexEntry new_node_e;

        new_buck.add(record);

        if (node.left == -1) {
            node.pos_bucket = -1;
            node.left = get_pos_add_node(indexFileFL); // count_index;
        } else {
            node.pos_bucket = -1;
            node.right = get_pos_add_node(indexFileFL); // count_index;
        }
        count_index++;

        new_node_e.pos_bucket = get_pos_add_bucket(dataFileFL); // metadata.count; 
        metadata.count++;

        // write bucket count and index count
        dataFile.seekp(0, std::ios::beg);
        dataFile.write((char*) &metadata.count, sizeof(int)); 
        indexFile.seekp(0, std::ios::beg);
        indexFile.write((char*) &count_index, sizeof(int)); 

        // write prev node updated
        indexFile.seekp(prev_pos_node * sizeof(IndexEntry) + sizeof(count_index), std::ios::beg);
        indexFile.write((char*) &node, sizeof(IndexEntry));

        // write new bucket
        dataFile.seekp(0, std::ios::end);
        new_buck.write(dataFile);

        // write new node
        indexFile.seekp(0, std::ios::end);
        indexFile.write((char*) &new_node_e, sizeof(IndexEntry));
    }

    indexFile.close();
    dataFile.close();
    indexFileFL.close();
    dataFileFL.close();
    return false;
}

template <typename TR, typename TK>
bool ExtHashFile<TR, TK>::remove(TK key) {
    std::fstream indexFile(indexfile, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFile.is_open())
        throw std::runtime_error("Cannot open indexfile"); 
    
    std::fstream dataFile(datafile, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 
    
    std::fstream indexFileFL(indexfilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFileFL.is_open())
        throw std::runtime_error("Cannot open index_fl file"); 
    
    std::fstream dataFileFL(datafilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFileFL.is_open())
        throw std::runtime_error("Cannot open data_fl file"); 

    if (metadata.count == 0)
        return false;

    int pos_bucket{};
    static TR record;
    int pos_record{};
    bool found {false};
    auto result = find_record(key, pos_bucket, record, pos_record, found);
    std::vector<int> nodes_to_delete{};
    int desc_depth{};

    if (result != nullptr && found == true) {
        std::string bin;
        std::vector<int> path = get_path_traversal(indexFile, key, bin);
        Bucket<TR> bucket(metadata.block_factor);
        dataFile.seekg((pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
        bucket.read(dataFile);

        if (bucket.metadata.count == 1) {
            IndexEntry node;
            int i{};
            for (i = path.size() - 2; i >= 0; i--) {
                indexFile.seekg((path[i] * sizeof(node)) + sizeof(count_index), std::ios::beg);
                indexFile.read((char*) &node, sizeof(node));
                if (node.left != -1 && node.right != -1) {
                    if (bin[i] == '0') {
                        nodes_to_delete.push_back(node.left);
                        node.left = -1;
                    } else {
                        nodes_to_delete.push_back(node.right);
                        node.right = -1;
                    }
                    break;
                } else {
                    if (node.right == -1)
                        nodes_to_delete.push_back(node.left);
                    else
                        nodes_to_delete.push_back(node.right);
                    desc_depth++;
                }
            }
            
            if (bucket.metadata.local_depth == metadata.global_depth) {
                metadata.global_depth = metadata.global_depth - desc_depth;
            }
            metadata.count--;
            count_index = count_index - nodes_to_delete.size();

            // update father node
            indexFile.seekp((path[i] * sizeof(node)) + sizeof(count_index), std::ios::beg);
            indexFile.write((char*) &node, sizeof(node));
            //indexFile.seekp(sizeof(int) + ((i + 1) * sizeof(node)), std::ios::beg);
            //indexFile.write((char*) &node, sizeof(node)); 
            
            // update metadata: global_depth, count
            dataFile.seekp(0, std::ios::beg);
            dataFile.write((char*) &metadata, sizeof(metadata));
            
            // update count_index 
            indexFile.seekp(0, std::ios::beg);
            indexFile.write((char*) &count_index, sizeof(count_index));

            // remove nodes
            remove_node(indexFileFL, nodes_to_delete);
            // remove bucket
            remove_bucket(dataFileFL, pos_bucket);

        } else {
            bucket.remove(pos_record);

            dataFile.seekp((pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
            dataFile.write((char*) &bucket.metadata, sizeof(bucket.metadata));
            for (int i = 0; i < bucket.metadata.capacity; ++i) {
                dataFile.write((char*) &bucket.next_del[i], sizeof(int));
            }
        }
        return true;
    }
    else {
        std::cout << "The record with that key does not exist or has already been deleted\n";
    }
    dataFile.close();
    indexFile.close();
    dataFileFL.close();
    indexFileFL.close();
    return false;
}

template <typename TR, typename TK>
void ExtHashFile<TR, TK>::find_node_pos(std::fstream& file, std::string& binary, int& pos_node, int& prev_pos_node, int& depth) {
    IndexEntry node; 
    int prev{};
    prev = pos_node; 

    file.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
    file.read((char*) &node, sizeof(IndexEntry));

    for (char bit: binary) {
        prev = pos_node;
        if (bit == '0') 
            pos_node = node.left;        
        else
            pos_node = node.right;

        if (pos_node == -1) 
            break;
        else {
            file.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
            file.read((char*) &node, sizeof(IndexEntry));
        }
        depth++;
    }
    prev_pos_node = prev;
}

template <typename TR, typename TK>
TR* ExtHashFile<TR, TK>::find_record(TK key, int& pos_bucket, TR& record, int& pos_record, bool& found) {
    std::fstream indexFile(indexfile, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFile.is_open())
        throw std::runtime_error("Cannot open indexfile"); 
    
    std::fstream dataFile(datafile, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 

    size_t num = hasher(key) % int(std::pow(2, metadata.global_depth));
    std::string binary = numToBin(num, metadata.global_depth);
    std::reverse(binary.begin(), binary.end());

    if (is_empty(indexFile)) {
        std::cout << "There are no records in the datafile\n";
        return nullptr;
        //throw ("There are no records in the datafile\n");
    }

    IndexEntry node;
    int prev_pos_node{};
    int pos_node{}; 
    indexFile.read((char*) &count_index, sizeof(count_index));
    int depth{};

    find_node_pos(indexFile, binary, pos_node, prev_pos_node, depth);

    if (pos_node == -1) {
        indexFile.seekg((prev_pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));

        if (node.is_leaf()) {
            Bucket<TR> bucket(metadata.block_factor);
            dataFile.seekg((node.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
            bucket.read(dataFile);

            for (int i = 0; i < bucket.metadata.capacity; ++i) {
                if (bucket.next_del[i] == 0 && bucket.records[i].get_key() == key) {
                    TR* record_ptr = &bucket.records[i];
                    pos_bucket = node.pos_bucket;
                    record = bucket.records[i];
                    pos_record = i;
                    found = true;
                    return record_ptr; 
                    //return bucket.records[i];
                }
            }
            std::cout << "No record was found with that key" << std::endl;
            return nullptr;
            //return {};
        } else {
            std::cout << "No record was found with that key" << std::endl;
            return nullptr;
            //return {};
        }
    } else {
        indexFile.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));

        Bucket<TR> bucket(metadata.block_factor);
        dataFile.seekg((node.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
        bucket.read(dataFile);

        for (int i = 0; i < bucket.metadata.capacity; ++i) {
            if (bucket.next_del[i] == 0 && bucket.records[i].get_key() == key) {
                TR* record_ptr = &bucket.records[i];
                pos_bucket = node.pos_bucket;
                record = bucket.records[i];
                pos_record = i;
                found = true;
                return record_ptr; 
                //return bucket.records[i];
            }
        }
        std::cout << "No record was found with that key" << std::endl;
        return nullptr;
        //return {};
    }
    indexFile.close();
    dataFile.close();
    return nullptr;
    //return {};
}

template <typename TR, typename TK>
void ExtHashFile<TR, TK>::add_record_rehashing(std::fstream& indexFile, std::fstream& dataFile, TR record) {
    // rehashing
    int num = hasher(record.get_key()) % int(std::pow(2, metadata.global_depth));
    std::string binary = numToBin(num, metadata.global_depth);
    std::reverse(binary.begin(), binary.end());

    IndexEntry node_e;
    int prev_pos_node = 0;
    int pos_node = 0; 
    int depth = 0;
    find_node_pos(indexFile, binary, pos_node, prev_pos_node, depth);
    
    // caso cuando despues de hacer split varias veces, el records se tiene que insertar en alguno 
    // de los bucket separados porque coinciden en prefijos de hash 
    if (pos_node != -1) { 
        indexFile.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node_e, sizeof(IndexEntry));
        Bucket<TR> buck(metadata.block_factor);
        dataFile.seekg((node_e.pos_bucket * buck.size_of()) + sizeof(metadata), std::ios::beg);
        buck.read(dataFile);
        buck.add(dataFile, node_e.pos_bucket, record);
    }
    else { 
        indexFile.seekg((prev_pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node_e, sizeof(IndexEntry));

        Bucket<TR> new_buck(metadata.block_factor);
        new_buck.metadata.local_depth = depth + 1;
        IndexEntry new_node_e;

        new_buck.add(record);

        if (node_e.left == -1) {
            node_e.pos_bucket = -1;
            node_e.left = count_index;
        } else {
            node_e.pos_bucket = -1;
            node_e.right = count_index;
        }
        count_index++;

        new_node_e.pos_bucket = metadata.count;
        metadata.count++;
        
        // write bucket count and index count
        dataFile.seekp(0, std::ios::beg);
        dataFile.write((char*) &metadata.count, sizeof(int)); 
        indexFile.seekp(0, std::ios::beg);
        indexFile.write((char*) &count_index, sizeof(int)); 

        // write prev node updated
        indexFile.seekp(prev_pos_node * sizeof(IndexEntry) + sizeof(count_index), std::ios::beg);
        indexFile.write((char*) &node_e, sizeof(IndexEntry));

        // write new bucket
        dataFile.seekp(0, std::ios::end);
        new_buck.write(dataFile);

        // write new node
        indexFile.seekp(0, std::ios::end);
        indexFile.write((char*) &new_node_e, sizeof(IndexEntry));
    }
}

template <typename TR, typename TK>
bool ExtHashFile<TR, TK>::is_empty(std::fstream& file) {
    return size(file) == 0;
}

template <typename TR, typename TK>
int ExtHashFile<TR, TK>::size(std::fstream& file) {
    file.seekg(0, std::ios::end); 
    std::streampos sz = file.tellg();
    file.seekg(0, std::ios::beg);
    return sz;
}

template <typename TR, typename TK>
int ExtHashFile<TR, TK>::capacity(std::fstream& dataFileFL) {
    int sz = size(dataFileFL);
    return (sz - sizeof(int)) / (sizeof(int));
}

template <typename TR, typename TK>
void ExtHashFile<TR, TK>::print() {
    std::fstream dataFile(datafile, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 

    std::fstream dataFileFL(datafilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFileFL.is_open())
        throw std::runtime_error("Cannot open data_fl file"); 

    dataFile.seekg(0, std::ios::beg);
    dataFile.read((char*) &metadata, sizeof(metadata));
    std::cout << "Metadata:" << std::endl;
    std::cout << "count: " << metadata.count << std::endl;
    std::cout << "global_depth: " << metadata.global_depth << std::endl;
    std::cout << "block_factor: " << metadata.block_factor << std::endl;

    for (int i = 0; i < capacity(dataFileFL); ++i) {
        Bucket<TR> bucket(metadata.block_factor);
        int next_d{};
        
        //                 header
        dataFileFL.seekg(sizeof(int) * (i + 1), std::ios::beg);
        dataFileFL.read((char*) &next_d, sizeof(next_d));
        if (next_d == 0) {
            dataFile.seekg((i * bucket.size_of()) + sizeof(metadata), std::ios::beg);
            bucket.read(dataFile);
            bucket.print();
        }
    }
    dataFile.close();
    dataFileFL.close();
}

template <typename TR, typename TK>
int ExtHashFile<TR, TK>::get_pos_add_bucket(std::fstream& dataFileFL) {
    int pos_header{};
    dataFileFL.seekg(0, std::ios::beg);
    dataFileFL.read((char*) &pos_header, sizeof(pos_header));
    
    if (pos_header == -1) { // 0 0 0 ...
        int add_new_pos = 0;
        dataFileFL.seekp(0, std::ios::end);
        dataFileFL.write((char*) &add_new_pos, sizeof(add_new_pos));
        return metadata.count;
    }
    else {
        int new_pos_header{};
        int set_no_del{};
        
        dataFileFL.seekg(sizeof(int) + pos_header, std::ios::beg);
        dataFileFL.read((char*) &new_pos_header, sizeof(new_pos_header));

        dataFileFL.seekp(sizeof(int) + pos_header, std::ios::beg);
        dataFileFL.write((char*) &set_no_del, sizeof(set_no_del));      

        dataFileFL.seekp(0, std::ios::beg);
        dataFileFL.write((char*) &new_pos_header, sizeof(new_pos_header));    
        
        return new_pos_header;
    }
}

template <typename TR, typename TK>
int ExtHashFile<TR, TK>::get_pos_add_node(std::fstream& indexFileFL) {
    int pos_header{};
    indexFileFL.seekg(0, std::ios::beg);
    indexFileFL.read((char*) &pos_header, sizeof(pos_header));
    
    if (pos_header == -1) { // 0 0 0 ...
        int add_new_pos = 0;
        indexFileFL.seekp(0, std::ios::end);
        indexFileFL.write((char*) &add_new_pos, sizeof(add_new_pos));
        return count_index;
    }
    else {
        int new_pos_header{};
        int set_no_del{};
        
        indexFileFL.seekg(sizeof(int) + pos_header, std::ios::beg);
        indexFileFL.read((char*) &new_pos_header, sizeof(new_pos_header));

        indexFileFL.seekp(sizeof(int) + pos_header, std::ios::beg);
        indexFileFL.write((char*) &set_no_del, sizeof(set_no_del));      

        indexFileFL.seekp(0, std::ios::beg);
        indexFileFL.write((char*) &new_pos_header, sizeof(new_pos_header));    
        
        return new_pos_header;
    }
}

template <typename TR, typename TK>
std::vector<int> ExtHashFile<TR, TK>::get_path_traversal(std::fstream& file, int key, std::string& bin) {
    std::vector<int> path;
    IndexEntry node; 
    int pos_node{};

    size_t num = hasher(key) % int(std::pow(2, metadata.global_depth));
    std::string binary = numToBin(num, metadata.global_depth);
    std::reverse(binary.begin(), binary.end());

    file.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
    file.read((char*) &node, sizeof(IndexEntry));
    path.push_back(pos_node); // root

    for (char bit: binary) {
        if (bit == '0') 
            pos_node = node.left;        
        else
            pos_node = node.right;

        if (pos_node == -1) 
            break;
        else {
            path.push_back(pos_node);
            bin += bit;
            file.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
            file.read((char*) &node, sizeof(IndexEntry));
        }
    }
    return path;
}


template <typename TR, typename TK>
void ExtHashFile<TR, TK>::remove_node(std::fstream& indexFileFL, std::vector<int> nodes_to_delete) {
    for (int i = 0; i < nodes_to_delete.size(); i++) {
        int pos_header{};
        indexFileFL.seekg(0, std::ios::beg);
        indexFileFL.read((char*) &pos_header, sizeof(pos_header));
        indexFileFL.seekp(sizeof(int) * (nodes_to_delete[i] + 1), std::ios::beg);
        indexFileFL.write((char*) &pos_header, sizeof(pos_header));
        indexFileFL.seekp(0, std::ios::beg);
        indexFileFL.write((char*) &nodes_to_delete[i], sizeof(nodes_to_delete[i]));
    }
}

template <typename TR, typename TK>
void ExtHashFile<TR, TK>::remove_bucket(std::fstream& dataFileFL, int pos_bucket) {
    int pos_header{};
    dataFileFL.seekg(0, std::ios::beg);
    dataFileFL.read((char*) &pos_header, sizeof(pos_header));
    dataFileFL.seekp(sizeof(int) * (pos_bucket + 1), std::ios::beg);
    dataFileFL.write((char*) &pos_header, sizeof(pos_header));
    dataFileFL.seekp(0, std::ios::beg);
    dataFileFL.write((char*) &pos_bucket, sizeof(pos_bucket));
}