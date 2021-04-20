#define N 30

#ifndef B_TREE_H
#define B_TREE_H

typedef struct {
	int x, y;
	char* string;
}Info;

typedef struct{
	const Info* info;
	int height;
}RetInfo;

typedef struct Node{
	char key[N];
	Info info;
	struct Node* left;
	struct Node* right;
	struct Node* par;
	struct Node* next;
	struct Node* prev;
}Node;

typedef struct{
	Node* root;
	int size;
	float alfa;
}Tree;

const Info* insert_tree(Node** root, int* max_size, char key[N], Info info);
const Info* find_min_tree(Node* root);
const Info* find_max_tree(Node* root);
const Info* find_tree(Node* root, char key[N]);
void remove_tree(Node** root, int* max_size, char key[N]);
void clear_tree(Node** root);
void print_tree(Node* root);
void format_print_tree(Node* node);
void write_tree(Node* root);
void read_tree(Node** root, int* max_size);
int depth_tree(Node* root);

void dot_print_tree(Node* root);

#endif