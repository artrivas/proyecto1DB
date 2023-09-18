
#include <iostream>
#include "IndexEntry.hpp"

IndexEntry::IndexEntry() {
	this->left = -1;
	this->right = -1;
	this->pos_bucket = -1;
}

IndexEntry::IndexEntry(int left, int right, int pos_bucket) {
	this->left = left;
	this->right = right;
	this->pos_bucket = pos_bucket;
}

void IndexEntry::print() {
	std::cout << "left: " << left << std::endl;
	std::cout << "right: " << right << std::endl;
	std::cout << "pos_bucket: " << pos_bucket << std::endl;
}

bool IndexEntry::is_leaf() {
	return left == -1 && right == -1;
}
