
struct IndexEntry {
 	int left;
	int right;
	int pos_bucket;
    
    IndexEntry();
    IndexEntry(int left, int right, int pos_bucket);
	void print();
	bool is_leaf();
};
