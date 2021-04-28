#ifndef FINDER_H
#define FINDER_H

typedef struct{
	unsigned int string_num;
	unsigned int offset;
}Info;

typedef struct Node{
	char* key;
	Info info;
	struct Node* left;
	struct Node* right;
	struct Node* par;
	struct Node* next;
	struct Node* prev;
}Node;

#endif