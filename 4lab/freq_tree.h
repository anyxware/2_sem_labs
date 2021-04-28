#ifndef FREQ_TREE_H
#define FREQ_TREE_H

typedef struct Node{
	char* key;
	int count;
	struct Node* left;
	struct Node* right;
	struct Node* par;
	struct Node* next;
	struct Node* prev;
}Node;

#endif