#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freq_tree.h"

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

Node* create_node(Node* par, char* key){
	Node* node = (Node*)malloc(sizeof(Node));

	node->key = (char*)malloc(strlen(key) + 1);
	strcpy(node->key, key);

	node->left = node->right = NULL;
	node->par = par;
	node->count = 1;
	return node;
}

void direct_node(Node* node){
	node->next = next_node(node);
	if(node->next) node->next->prev = node;
	node->prev = prev_node(node);
	if(node->prev) node->prev->next = node;
}

void insert_tree(Node** root, char* key){
	if(!*root){
		*root = create_node(NULL, key);
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
				node->right = create_node(node, key);
				direct_node(node->right);
				return;
			}
		}else if(status < 0){
			if(node->left){
				node = node->left;
			}else{
				node->left = create_node(node, key);
				direct_node(node->left);
				return;
			}
		}else{
			node->count++;
			return;
		}
	}
}

void clear_node(Node* node){
	if(node){
		//list_clear(&node->list);
		clear_node(node->left);
		clear_node(node->right);
		free(node->key);
		free(node);
	}
}

void clear_tree(Node** root){
	clear_node(*root);
	*root = NULL;
}

void print_node(Node* node){
	if(node){
		printf("%s: ", node->key);
		printf("%d\n", node->count);
		print_node(node->next);
	}
}

void print_tree(Node* root){
	Node* start = find_min_node(root);
	print_node(start);
}

int isSymbol(char token){
	return 'a' <= token && token <= 'z' || 'A' <= token && token <= 'Z';
}

void inserter(Node** root, int counter, FILE* file, int* word, int pad){
	if(!*word){
		return;
	}
	char* key = (char*)malloc(counter + 1);
	fseek(file, -counter - pad, SEEK_CUR);
	fread(key, sizeof(char), counter, file);
	key[counter] = '\0';
	for(int j = 0; j < counter; j++){
		key[j] = ('a' <= key[j] && key[j] <= 'z') * key[j] + ('A' <= key[j] && key[j] <= 'Z') * (key[j] - 'A' + 'a');
	}
	insert_tree(root, key);
	fseek(file, 1, SEEK_CUR);
	*word = 0;
	free(key);
}

void parser(FILE* file, Node** root){
	int word = 0;
	int counter;
	char token;
	while((token = getc(file)) != EOF){
		if(isSymbol(token) && !word){
			word = 1;
			counter = 1;
		}else if(isSymbol(token) && word){
			counter++;
		}else{
			inserter(root, counter, file, &word, 1);
		}
	}
	inserter(root, counter, file, &word, 0);
}

int main(int argc, char const *argv[])
{
	Node* root = NULL;

	FILE* file = fopen("test.txt", "r");
	if(!file){
		perror("fopen()");
		return EXIT_SUCCESS;
	}
	parser(file, &root);
	print_tree(root);
	clear_tree(&root);
	fclose(file);
	return 0;
}