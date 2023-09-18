//
// Created by artrivas on 9/12/23.
//

#ifndef UNTITLED_AVLNODE_H
#define UNTITLED_AVLNODE_H
template<typename KeyType>
struct AVLNode{
    KeyType key{};
    long file_position = -1;
    long left = -1;
    long right = -1;
    long height = 0;
    long list_pointer = -1;
    AVLNode(KeyType key, long file_position){
        strcpy(this->key,key); //Tiene que ser generico para int
        this->file_position = file_position;
        this->left = -1;
        this->right = -1;
        this->list_pointer = -1;
        this->height = 1;
    }
    AVLNode() = default;
};

#endif //UNTITLED_AVLNODE_H