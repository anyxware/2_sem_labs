#include "list.h"
#define N 6

#ifndef NTREE_H
#define NTREE_H

typedef struct Node{
	char key[N];
	LIST list;
	struct Node* left;
	struct Node* right;
	struct Node* par;
	struct Node* next;
	struct Node* prev;
}Node;

void insert_tree(Node** root, char key[N], char* string);
char** find_min_tree(Node* root);
char** find_max_tree(Node* root);
char** find_tree(Node* root, char key[N]);
void remove_tree(Node** root, char key[N]);
void clear_tree(Node** root);
void print_tree(Node* root);
void format_print_tree(Node* node);
void write_tree(Node* root);
void read_tree(Node** root);
int depth_node(Node* root);

void dot_print_tree(Node* root);

#endif