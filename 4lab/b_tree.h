#define N 30
#define BUF_SIZE 4

#ifndef B_TREE_H
#define B_TREE_H

typedef struct {
	int x, y;
	char* string;
}Info;

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
	Info info;
	int height;
	Node* new_node;
}RetInfo;

typedef struct{
	Node* node;
	unsigned int age;
	int busy;
}BufItem;

typedef struct{
	BufItem array[3 * BUF_SIZE];
	unsigned int max_age;
}Bufer;

typedef struct{
	Node* root;
	int max_size;
	float alfa;
	Bufer bufer;
}Tree;

Info insert_tree(Tree* tree, char key[N], Info info);
const Info* find_min_tree(Node* root);
const Info* find_max_tree(Node* root);
const Info* find_tree(Tree* tree, char key[N]);
void remove_tree(Tree* tree, char key[N]);
void clear_tree(Tree* tree);
void print_tree(Node* root);
void format_print_tree(Node* node);
void write_tree(Tree tree);
void read_tree(Tree* tree);
int depth_tree(Node* root);
void dot_print_tree(Node* root);

#endif