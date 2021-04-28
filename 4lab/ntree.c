#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "ntree.h"

Node* find_min_node(Node* node){
	for(;node->left; node = node->left){}
	return node;
}

Node* find_max_node(Node* node){
	for(;node->right; node = node->right){}
	return node;
}

Node* next_node(Node* node){
	if(node->right){
		return find_min_node(node->right);
	}
	Node* par = node->par;
	for(;par && node == par->right; node = par, par = par->par){}
	return par;
}

Node* prev_node(Node* node){
	if(node->left){
		return find_max_node(node->left);
	}
	Node* par = node->par;
	for(;par && node == par->left; node = par, par = par->par){}
	return par;
}

Node* create_node(Node* par, char key[N], char* string){
	Node* node = (Node*)malloc(sizeof(Node));
	strcpy(node->key, key);
	node->left = node->right = NULL;
	node->par = par;
	node->list.head = NULL;
	node->list.tail = NULL;
	list_append(&node->list, (void*)string, strlen(string) + 1);

	return node;
}

void direct_node(Node* node){
	node->next = next_node(node);
	if(node->next) node->next->prev = node;
	node->prev = prev_node(node);
	if(node->prev) node->prev->next = node;
}

void insert_tree(Node** root, char key[N], char* string){
	if(!*root){
		*root = create_node(NULL, key, string);
		direct_node(*root);
		return;
	}
	Node* node = *root;
	int status;
	while(node){
		status = strcmp(key, node->key);
		if(status > 0){
			if(node->right){
				node = node->right;
			}else{
				node->right = create_node(node, key, string);
				direct_node(node->right);
				return;
			}
		}else if(status < 0){
			if(node->left){
				node = node->left;
			}else{
				node->left = create_node(node, key, string);
				direct_node(node->left);
				return;
			}
		}else{
			list_append(&node->list, (void*)string, strlen(string) + 1);
			return;
		}
	}
}

char** find_min_tree(Node* node){
	Node* find = find_min_node(node);
	if(!find) return NULL;
	return list_return_vector_with_strings(&find->list);
}

char** find_max_tree(Node* node){
	Node* find = find_max_node(node);
	if(!find) return NULL;
	return list_return_vector_with_strings(&find->list);
}

Node* find_node(Node* node, char key[N]){
	int status;
	while(node){
		status = strcmp(key, node->key);
		if(status > 0){
			node = node->right;
		}else if(status < 0){
			node = node->left;
		}else{
			return node;
		}
	}
	return NULL;
}

char** find_tree(Node* root, char key[N]){
	Node* find = find_node(root, key);
	if(!find) return NULL;
	return list_return_vector_with_strings(&find->list);
}

void remove_node(Node** element, Node* target){
	Node* node = *element;
	if(!target){
		void* tmp = list_return(&node->list, 0);
		free(tmp);
	}
	if(node->list.head && !target){
		return;
	}
	if(node->left && node->right){
		Node* max = find_max_node(node->left);
		strcpy(node->key, max->key);
		node->list.head = max->list.head;
		node->list.tail = max->list.tail;

		if(node->prev) node->prev->next = node->next;
		if(node->next) node->next->prev = node->prev;

		remove_node(&max, node);
		return;
	}else if(node->left){
		if(!node->par){
			*element = node->left;
			node->left->par = NULL;
		}else if(node == node->par->left){
			node->par->left = node->left;
			node->left->par = node->par;
		}else{
			node->par->right = node->left;
			node->left->par = node->par;
		}
	}else if(node->right){
		if(!node->par){
			*element = node->right;
			node->right->par = NULL;
		}else if(node == node->par->left){
			node->par->left = node->right;
			node->right->par = node->par;
		}else{
			node->par->right = node->right;
			node->right->par = node->par;
		}
	}else{
		if(!node->par){
			*element = NULL;
		}else if(node == node->par->left){
			node->par->left = NULL;
		}else{
			node->par->right = NULL;
		}
	}
	if(!target) list_clear(&node->list);
	if(target){
		if(node->prev) node->prev->next = target;
		if(node->next) node->next->prev = target;
	}else{
		if(node->prev) node->prev->next = node->next;
		if(node->next) node->next->prev = node->prev;
	}
	free(node);
}

void remove_tree(Node** root, char key[N]){
	Node* node = find_node(*root, key);
	if(!node) return;
	if(node == *root) remove_node(root, NULL);
	else remove_node(&node, NULL);
}

void clear_node(Node* node){
	if(node){
		list_clear(&node->list);
		clear_node(node->left);
		clear_node(node->right);
		free(node);
	}
}

void clear_tree(Node** root){
	clear_node(*root);
	*root = NULL;
}

void print_node(Node* node){
	while(node){
		printf("%s: ", node->key);
		list_print(&node->list);
		node = node->next;
	}
}

void print_tree(Node* root){
	Node* start = find_min_node(root);
	print_node(start);
}

void format_print_node(Node* node, int pad, int dir, char* pos){
	int i;

	for(i = 0; i < pad; i++){
		if((pad - i) % 4 == 0 && pos[i / 4] == 1) printf("\033[1;33m│\033[0m");
		else printf(" ");
	}
	int next_dash;
	if(dir){
		printf("\033[1;32m└──>\033[0m");
		pos[i / 4] = 0;
	}
	else {
		printf("\033[1;35m├──>\033[0m");
		pos[i / 4] = 1;
	}
	if(node){
		printf("%s: ", node->key);
		list_print(&node->list);
		if(node->left || node->right){
			format_print_node(node->left, pad + 4, 0, pos);
			format_print_node(node->right, pad + 4, 1, pos);
		}
	}else{
		printf("\033[1;31m-\033[0m\n");
	}
}

int height_node(Node* node){
	if(!node){
		return 0;
	}else{
		int left_height = height_node(node->left);
		int right_height = height_node(node->right);
		return left_height > right_height ? left_height + 1 : right_height + 1;
	}
}

void format_print_tree(Node* root){
	int depth = height_node(root);
	char* pos = (char*)calloc(1, depth);
	format_print_node(root, 0, 1, pos);
	free(pos);
}

void dot_print_node(Node* node, FILE* file){
	if(node){
		char arrow[] = " -> ";
		char c = ';';
		char null[] = "null";
		char shape[] = "[shape=point];";

		if(node->left || node->right)
		{


		if(!node->left){
			fwrite(null, sizeof(char), strlen(null), file);
			fwrite(node->key, sizeof(char), strlen(node->key), file);
			fwrite(shape, sizeof(char), strlen(shape), file);
		}
		fwrite(node->key, sizeof(char), strlen(node->key), file);
		fwrite(arrow, sizeof(char), strlen(arrow), file);
		if(node->left) {
			fwrite(node->left->key, sizeof(char), strlen(node->left->key), file);
		}
		else{
			fwrite(null, sizeof(char), strlen(null), file);
			fwrite(node->key, sizeof(char), strlen(node->key), file);
		}
		fwrite(&c, sizeof(char), 1, file);
		

		if(!node->right){
			fwrite(null, sizeof(char), strlen(null), file);
			fwrite(node->key, sizeof(char), strlen(node->key), file);
			fwrite(shape, sizeof(char), strlen(shape), file);
		}
		fwrite(node->key, sizeof(char), strlen(node->key), file);
		fwrite(arrow, sizeof(char), strlen(arrow), file);
		if(node->right) {
			fwrite(node->right->key, sizeof(char), strlen(node->right->key), file);
		}
		else{
			fwrite(null, sizeof(char), strlen(null), file);
			fwrite(node->key, sizeof(char), strlen(node->key), file);
		}
		fwrite(&c, sizeof(char), 1, file);


		dot_print_node(node->left, file);
		dot_print_node(node->right, file);
		}
	}
}

void dot_print_tree(Node* root){
	FILE* file = fopen("ntree.dot", "w");
	char s[] = "digraph G {graph [ dpi = 150 ];nodesep=0.3;ranksep=0.2;margin=0.1;node [shape=circle];edge [arrowsize=0.8];";
	fwrite(s, sizeof(char), strlen(s), file);
	dot_print_node(root, file);
	char c = '}';
	fwrite(&c, sizeof(char), 1, file);
	fclose(file);
}

void write_node(Node* node, FILE* file){
	if(node){
		while(node->list.head){
			void* tmp = list_return(&node->list, 0);
			char* string = (char*)tmp;
			int len = strlen(string);
			fwrite(&len, sizeof(int), 1, file);
			fwrite(node->key, sizeof(char), strlen(node->key), file);
			char c = 0;
			fwrite(&c, sizeof(char), N - strlen(node->key), file);
			fwrite(string, sizeof(char), len, file);
			free(tmp);
		}
		write_node(node->left, file);
		write_node(node->right, file);
	}
}

void write_tree(Node* root){
	FILE* file = fopen("ntree.bin", "wb");
	write_node(root, file);
	int end_len = 0;
	fwrite(&end_len, sizeof(int), 1, file);
	fclose(file);
}

void read_tree(Node** root){
	FILE* file = fopen("ntree.bin", "rb");
	int len;
	char key[N];
	char* string;
	for(;;){
		fread(&len, sizeof(int), 1, file);
		if(!len){
			break;
		}
		fread(key, sizeof(char), N, file);
		string = (char*)malloc(len + 1);
		fread(string, sizeof(char), len, file);
		string[len] = '\0';
		insert_tree(root, key, string);
		free(string);
	}
	fclose(file);
}