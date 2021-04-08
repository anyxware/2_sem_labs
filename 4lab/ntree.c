#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "ntree.h"

Node* create_node(Node* par, char key[N], char* string){
	Node* node = (Node*)malloc(sizeof(Node));
	strcpy(node->key, key);
	node->left = node->right = NULL;
	node->list.head = NULL;
	node->list.tail = NULL;
	list_append(&node->list, (void*)string, strlen(string) + 1);
	node->par = par;
	return node;
}

void insert_tree(Node** root, char key[N], char* string){
	if(!*root){
		*root = create_node(NULL, key, string);
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
				return;
			}
		}else if(status < 0){
			if(node->left){
				node = node->left;
			}else{
				node->left = create_node(node, key, string);
				return;
			}
		}else{
			list_append(&node->list, (void*)string, strlen(string) + 1);
		}
	}
}

Node* find_min_node(Node* node){
	for(;node->left; node = node->left){}
	return node;
}

char** find_min_tree(Node* node){
	Node* find = find_min_node(node);
	return list_return_vector_with_strings(&find->list);
}

Node* find_max_node(Node* node){
	for(;node->right; node = node->right){}\
	return node;
}

char** find_max_tree(Node* node){
	Node* find = find_max_node(node);
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
	return list_return_vector_with_strings(&find->list);
}

void remove_node(Node** element, int freed){
	Node* node = *element;
	if(!freed){
		void* tmp = list_return(&node->list, 0);
		free(tmp);
	}
	if(node->list.head && !freed){
		return;
	}
	if(node->left && node->right){
		Node* max = find_max_node(node->left);
		strcpy(node->key, max->key);
		node->list.head = max->list.head;
		node->list.tail = max->list.tail;
		remove_node(&max, 1);
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
	if(!freed) list_clear(&node->list);
	free(node);
}

void remove_tree(Node** root, char key[N]){
	Node* node = find_node(*root, key);
	if(node == *root) remove_node(root, 0);
	else remove_node(&node, 0);
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

void print_node(Node* node){
	if(node){
		printf("%s: ", node->key);
		list_print(&node->list);
		print_node(next_node(node));
	}
}

void print_tree(Node* root){
	Node* start = find_min_node(root);
	print_node(start);
}
