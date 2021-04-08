#include "list.h"
#define N 30

#ifndef WTABLE_H
#define WTABLE_H

typedef struct Node{
	char key[N];
	LIST list;
	struct Node* left;
	struct Node* right;
	struct Node* par;
}Node;

void insert_tree(Node** root, char key[N], char* string);
char** find_min_tree(Node* root);
char** find_max_tree(Node* root);
char** find_tree(Node* root, char key[N]);
void remove_tree(Node** root, char key[N]);
void clear_tree(Node** root);
void print_tree(Node* root);

#endif