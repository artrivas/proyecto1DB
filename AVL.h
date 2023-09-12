//
// Created by artrivas on 9/12/23.
//

#ifndef UNTITLED_AVL_H
#define UNTITLED_AVL_H
#include "AVLNode.h"
#include <iostream>
#include<fstream>
#include<vector>
#include "Record.h"
using namespace std;
template<typename KeyType,int attrpos>
class AVLFile
{
    string indexfilename;
    string heapfilename;
    ifstream readfile;
    ofstream writefile;
    long pos_root{};
    //8,20,20,20,5,4,11,15,25,4,4,4
    vector<int> prefix_sum = {0,8,28,48,68,73,77,88,103,128,132,136}; //Aca me puede agarrar heider
public:
    explicit AVLFile(const string & indexfilename, const string & heapfilename){
        this->pos_root = 1;
        this->indexfilename = indexfilename;
        this->heapfilename = heapfilename;
        this->writefile.open(this->indexfilename,ios::binary | ios::in);
        this->readfile.open(this->indexfilename,ios::binary);
    }
    void setup_files(){
        this->readfile.close();
        this->writefile.close();
        this->writefile.open(this->indexfilename,ios::binary | ios::in);
        this->readfile.open(this->indexfilename,ios::binary);
    }
    AVLNode<KeyType> create_node(long pos) {
        ifstream file(this->heapfilename,ios::binary);
        file.seekg((pos-1)*sizeof(Record) + prefix_sum[attrpos]);
        KeyType data;
        file.read((char*) &data,sizeof(data));
        file.close();
        AVLNode<KeyType> node(data,pos);
        return node;
    }

    void insert(Record record) {
        setup_files();
        ofstream file(this->heapfilename,ios::binary | ios::app);
        file.write((char *) &record, sizeof(Record));
        long nodepos = file.tellp()/sizeof(Record);
        file.close();
        add(nodepos);
    }

    vector<Record> find(KeyType & key){
        vector<AVLNode<KeyType>> arr = find(pos_root,key);
        ifstream file(heapfilename,ios::binary);
        vector<Record> ans;
        for(auto i :arr){
            file.seekg((i.file_position-1)*sizeof(Record));
            Record a;
            file.read((char*) &a,sizeof(Record));
            ans.push_back(a);
        }
        file.close();
        return ans;
    }

private:

    vector<AVLNode<KeyType>> find(long pos_node, KeyType & key){
        if(pos_node<0) return vector<AVLNode<KeyType>>{};
        readfile.seekg((pos_node-1)*sizeof(AVLNode<KeyType>));
        AVLNode<KeyType> cmp_node;
        readfile.read((char*) &cmp_node,sizeof(AVLNode<KeyType>));
        int result = strcmp(key,cmp_node.key);
        cout<< "llave: " << cmp_node.key << ' ' << key << result << ' ' << pos_node << ' ' << cmp_node.left << '\n';
        if(result > 0){
            return find(cmp_node.right,key);
        }else if(result <0){
            return find(cmp_node.left,key);
        }else{
            //Tendria que hacer una busqueda de arriba y abajo para obtener todos los records que cumplen
            vector<AVLNode<KeyType>> ans;
            ans.push_back(cmp_node);
            AVLNode<KeyType> cnt = get_node(cmp_node.list_pointer);
            while(cnt.file_position != -1){
                ans.push_back(cnt);
                cnt = get_node(cnt.list_pointer);
            }
            return ans;
        }
        return vector<AVLNode<KeyType>>{};
    }

    void left_rotation(AVLNode<KeyType> & node, long position){
        AVLNode<KeyType> y = get_node(node.right);
        AVLNode<KeyType> T2 = get_node(y.left);
        long y_pos = node.right;
        long T2pos = y.left;
        y.left = position;
        if(T2.file_position == -1){
            node.right = -1;
        }else{
            node.right = T2pos;
        }
        update_height(node,position);
        update_height(y,y_pos);
        y.left = y_pos;
        writefile.seekp((position-1)*sizeof(AVLNode<KeyType>));
        writefile.write((char*) &y,sizeof(y));
        writefile.seekp((y_pos-1)*sizeof(AVLNode<KeyType>));
        writefile.write((char*) &node,sizeof(node));
    }

    void right_rotation(AVLNode<KeyType> & node, long position){
        AVLNode<KeyType> x = get_node(node.left);
        AVLNode<KeyType> T2 = get_node(x.right);
        long x_pos = node.left;
        long T2_pos = x.right;
        x.right = position;
        if(T2.file_position == -1){
            node.left = -1;
        }else{
            node.left = T2_pos;
        }
        update_height(node,position);
        update_height(x,x_pos);
        x.right = x_pos;
        writefile.seekp((position-1)*sizeof(AVLNode<KeyType>));
        writefile.write((char*) &x,sizeof(AVLNode<KeyType>));
        writefile.seekp((x_pos-1)*sizeof(AVLNode<KeyType>));
        writefile.write((char*) &node,sizeof(AVLNode<KeyType>));
    }

    void push_node(long pos_node,AVLNode<KeyType> & node, AVLNode<KeyType> & cmp_node, short direction){
        writefile.seekp(0,ios::end);
        if(direction == 1) cmp_node.right = 1+writefile.tellp()/sizeof(AVLNode<KeyType>);
        else if(!direction) cmp_node.left = 1+writefile.tellp()/sizeof(AVLNode<KeyType>);
        else {
            node.list_pointer = cmp_node.list_pointer;
            cmp_node.list_pointer = 1+writefile.tellp()/sizeof(AVLNode<KeyType>);
        }
        writefile.write((char*) &node,sizeof(AVLNode<KeyType>));
        writefile.seekp((pos_node-1)*sizeof(AVLNode<KeyType>));
        writefile.write((char*) &cmp_node,sizeof(AVLNode<KeyType>));
    }

    AVLNode<KeyType> get_node(long posnode){
        AVLNode<KeyType> node;
        if(posnode == -1) return node;
        readfile.seekg((posnode-1)*sizeof(node));
        readfile.read((char*) & node, sizeof(node));
        return node;
    }

    void update_height(AVLNode<KeyType> & node, long pos){
        setup_files();
        node.height = max(
                get_node(node.right).height,
                get_node(node.left).height
        ) + 1;
        writefile.seekp((pos-1)*sizeof(AVLNode<KeyType>));
        writefile.write((char*) &node,sizeof(node));
    }

    void balance_tree(AVLNode<KeyType> & cmp_node, AVLNode<KeyType> & node,long position){
        AVLNode<KeyType> left_node = get_node(cmp_node.left);
        AVLNode<KeyType> right_node = get_node(cmp_node.right);
        int balancing_factor = left_node.height - right_node.height;
        int resultleft = strcmp(node.key,left_node.key);
        int resultright = strcmp(node.key,right_node.key);
        if(balancing_factor > 1 && resultleft < 0){//left left
            right_rotation(cmp_node,position);
        }else if(balancing_factor < -1 &&  0 < resultright){ //right right
            left_rotation(cmp_node,position);
        }else if(balancing_factor > 1 && 0 < resultleft){ //Existe un nodo a la izquierda
            left_rotation(left_node, cmp_node.left);
            right_rotation(cmp_node,position);
        }else if(balancing_factor < -1 && resultright < 0){
            right_rotation(right_node,cmp_node.right);
            left_rotation(cmp_node,position);
        }
    }

    void _add(long pos_node,AVLNode<KeyType> & node){
        readfile.seekg((pos_node-1)*sizeof(AVLNode<KeyType>));
        AVLNode<KeyType> cmp_node;
        readfile.read((char*) & cmp_node, sizeof(cmp_node));
        int result = strcmp(node.key,cmp_node.key);
        if(result > 0){
            if(cmp_node.right == -1){
                push_node(pos_node,node,cmp_node,1);
                cout << cmp_node.left << ' ' << cmp_node.right  << ' ' << cmp_node.key<< '\n';
            }else{
                _add(cmp_node.right,node);
            }
        }else if(result < 0){
            if(cmp_node.left == -1){
                push_node(pos_node,node,cmp_node,0);
                cout << cmp_node.left << ' ' << cmp_node.right  << ' ' << cmp_node.key<< '\n';
            }else{
                _add(cmp_node.left,node);
            }
        }else{
            //El caso en donde se repita las variables en caso no sea llave primaria;
            //De alguna forma tengo que saber si la llave es primaria
            push_node(pos_node,node,cmp_node,2);
            return;
        }
        update_height(cmp_node,pos_node);
        balance_tree(cmp_node,node,pos_node);
    }

    //El método add inserta el registro despues de la insercion en el heapfile, si es un scanall, no habria necesidad
    void add(long fileposition){ //Este metodo asume que ya existen registros en el heapfile
        bool valid = readfile.peek() == std::ifstream::traits_type::eof(); //El archivo esta vacio (?)
        AVLNode<KeyType> node = create_node(fileposition);
        if(valid){ //Si lo esta entonces solo insertamos el nodo
            writefile.write((char*) &node, sizeof(AVLNode<KeyType>));
            writefile.flush(); //Ver si funciona sin esto
        }else{ //De lo contrario lo insertamos recursivamente
            _add(pos_root,node);
        }
    }


};

#endif //UNTITLED_AVL_H
