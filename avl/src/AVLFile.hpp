
#pragma once
#include "AVLNode.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "./records/Player.hpp"

using Record = Player;

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
        unsigned long nodepos = file.tellp()/sizeof(Record);
        file.close();
        add(nodepos);
    }

    vector<Record> find(KeyType & key){
        AVLNode<KeyType> root_node = get_node(this->pos_root);
        if(root_node.file_position == -1)
            return vector<Record>{};
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

    bool remove(KeyType & key) {
        AVLNode<KeyType> root = get_node(this->pos_root);
        long ans = _remove(key,root,this->pos_root);
        if(ans == -2) return false;
        return true;
    }


    long find_succesor(AVLNode<KeyType> cmp_node,long pos){
        if(cmp_node.left == -1)
            return pos;
        AVLNode<KeyType> left_node = get_node(cmp_node.left);
        find_succesor(left_node,cmp_node.left);
    }

    long get_balance(long pos){
        AVLNode<KeyType> node = get_node(pos);
        AVLNode<KeyType> left_node = get_node(node.left);
        AVLNode<KeyType> right_node = get_node(node.right);
        return left_node.height-right_node.height;
    }

    //Eliminar un nodo es seteando su valor a -1
    long _remove(KeyType key,AVLNode<KeyType> cmp_node,long pos){
        if(cmp_node.file_position == -1) return -2;
        int result1 = strcmp(cmp_node.key,key);
        if(result1 > 0){
            AVLNode<KeyType> left_node = get_node(cmp_node.left);
            int result2 = strcmp(left_node.key,key);
            if(!result2){
                cmp_node.left = _remove(key,left_node,cmp_node.left);
                insert_node_by_pos(cmp_node,pos);
            }else{
                _remove(key,left_node,cmp_node.left);
            }
        }else if(result1 < 0){
            AVLNode<KeyType> right_node = get_node(cmp_node.right);
            int result3 = strcmp(right_node.key,key);
            if(!result3){
                cmp_node.right = _remove(key,right_node,cmp_node.right);
                insert_node_by_pos(cmp_node,pos);
            }else{
                _remove(key,right_node,cmp_node.right);
            }
        }else{
            if(cmp_node.left == -1 || cmp_node.right == -1){ //El nodo tiene 1 hijo o es nodo hoja
                cmp_node.file_position = -1;
                insert_node_by_pos(cmp_node,pos);
                return (cmp_node.left == -1 ? cmp_node.right : cmp_node.left); //Si cmp_node.right es -1, no tiene hijos |
            }else{ // Nodo con 2 hijos
                AVLNode<KeyType> right_subtree = get_node(cmp_node.right);
                long pos_succesor = find_succesor(right_subtree,cmp_node.right);
                AVLNode<KeyType> succesor = get_node(pos_succesor);
                memcpy((char*) & cmp_node.key, (char*) & succesor.key, sizeof(cmp_node.key));
                cmp_node.right = _remove(succesor.key,right_subtree,cmp_node.right);
                insert_node_by_pos(cmp_node,pos);
            }
        }
        //Actualizar alturas
        update_height(cmp_node,pos);

        long balance = get_balance(pos);

        if (balance > 1 && get_balance(cmp_node.left) >= 0) // Left Left
            right_rotation(cmp_node,pos);
        else if (balance > 1 && get_balance(cmp_node.left) < 0){ // Left Right
            AVLNode<KeyType> left_node = get_node(cmp_node.left);
            left_rotation(left_node,cmp_node.left);
            right_rotation(cmp_node,pos);
        }else if (balance < -1 && get_balance(cmp_node.right) <= 0) //Right Right
            left_rotation(cmp_node,pos);
        else if (balance < -1 && get_balance(cmp_node.right) > 0){// Right Left
            AVLNode<KeyType> right_node = get_node(cmp_node.right);
            right_rotation(right_node,cmp_node.right);
            left_rotation(cmp_node,pos);
        }

        return pos;
    }

    vector<Record> rangeSearch(KeyType & bkey, KeyType & ekey){
        AVLNode<KeyType> root_node;
        readfile.seekg((pos_root-1)*sizeof(AVLNode<KeyType>));
        readfile.read((char *) &root_node,sizeof(AVLNode<KeyType>));
        vector<AVLNode<KeyType>> ans;
        _rangeSearch(root_node,bkey,ekey,ans);
        ifstream file(this->heapfilename,ios::binary);
        vector<Record> arr;
        for(auto i :ans){
            file.seekg((i.file_position-1)*sizeof(Record));
            Record a;
            file.read((char*) &a,sizeof(Record));
            arr.push_back(a);
        }
        return arr;
    }
private:

    void _rangeSearch(AVLNode<KeyType> cmp_node, KeyType & bkey, KeyType & ekey,vector<AVLNode<KeyType>> & ans){
        if(cmp_node.file_position == -1)
            return;
        int result1 = strcmp(bkey,cmp_node.key); //bkey <= cmp.key
        int result2 = strcmp(cmp_node.key,ekey); //cmp.key <= ekey
        if(result1<=0){
            AVLNode<KeyType> new_cmp_node;
            readfile.seekg((cmp_node.left-1)*sizeof(AVLNode<KeyType>));
            readfile.read((char*) &new_cmp_node,sizeof(AVLNode<KeyType>));
            _rangeSearch(new_cmp_node,bkey,ekey,ans);
        }
        if(result1 <= 0 && result2 <=0){
            //Iprimir;
            ans.push_back(cmp_node);
            AVLNode<KeyType> cnt = get_node(cmp_node.list_pointer);
            while(cnt.file_position != -1){
                ans.push_back(cnt);
                cnt = get_node(cnt.list_pointer);
            }
        }
        if(result2 <=0){
            AVLNode<KeyType> new_cmp_node;
            readfile.seekg((cmp_node.right-1)* sizeof(AVLNode<KeyType>));
            readfile.read((char*)&new_cmp_node,sizeof(AVLNode<KeyType>));
            _rangeSearch(new_cmp_node,bkey,ekey,ans);
        }
    }
    vector<AVLNode<KeyType>> find(long pos_node, KeyType & key){
        if(pos_node<0) return vector<AVLNode<KeyType>>{};
        readfile.seekg((pos_node-1)*sizeof(AVLNode<KeyType>));
        AVLNode<KeyType> cmp_node;
        readfile.read((char*) &cmp_node,sizeof(AVLNode<KeyType>));
        int result = strcmp(key,cmp_node.key);
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
    void insert_node_by_pos(AVLNode<KeyType> & node, long position){
        setup_files();
        writefile.seekp((position-1)*sizeof(AVLNode<KeyType>));
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
        setup_files(); //Porque es necesario (?)
        AVLNode<KeyType> node;
        if(posnode == -1) return node;
        readfile.seekg((posnode-1)*sizeof(node));
        readfile.read((char*) & node, sizeof(AVLNode<KeyType>));
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
        //Balance tree
        AVLNode<KeyType> left_node = get_node(cmp_node.left);
        AVLNode<KeyType> right_node = get_node(cmp_node.right);
        int balancing_factor = left_node.height - right_node.height;
        int resultleft = strcmp(node.key,left_node.key);
        int resultright = strcmp(node.key,right_node.key);
        if(balancing_factor > 1 && resultleft < 0){//left left
            right_rotation(cmp_node,pos_node);
        }else if(balancing_factor < -1 &&  0 < resultright){ //right right
            left_rotation(cmp_node,pos_node);
        }else if(balancing_factor > 1 && 0 < resultleft){ //Existe un nodo a la izquierda
            left_rotation(left_node, cmp_node.left);
            right_rotation(cmp_node,pos_node);
        }else if(balancing_factor < -1 && resultright < 0){
            right_rotation(right_node,cmp_node.right);
            left_rotation(cmp_node,pos_node);
        }
    }

    //El método add inserta el registro despues de la insercion en el heapfile, si es un scanall, no habria necesidad
    void add(long fileposition){ //Este metodo asume que ya existen registros en el heapfile
        bool valid = readfile.peek() == std::ifstream::traits_type::eof(); //El archivo esta vacio (?)
        AVLNode<KeyType> node = create_node(fileposition);
        if(valid){ //Si lo esta entonces solo insertamos el nodo
            writefile.write((char*) &node, sizeof(AVLNode<KeyType>));
            writefile.flush(); //Ver si funciona sin esto
        }else{ //De lo contrario lo insertamos recursivamente
            AVLNode<KeyType> root_node = get_node(this->pos_root);
            if(root_node.file_position == -1)
                writefile.write((char*) &node, sizeof(AVLNode<KeyType>));
            else
                _add(pos_root,node);
        }
    }


};

