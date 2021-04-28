#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "finder.h"
#include "task.h"

float log_alfa(float alfa, int x){
	return (logf((float)x) / logf(1 / (float)alfa));
}

Node* find_min_node(Node* node){
	for(;node->left; node = node->left){}
	return node;
}

Node* find_max_node(Node* node){
	for(;node->right; node = node->right){}
	return node;
}

Node* find_node(Node* node, char* key){
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

Node* create_node(Node* par, char* key, Info info){
	Node* node = (Node*)malloc(sizeof(Node));
	node->key = (char*)malloc(strlen(key) + 1);
	strcpy(node->key, key);
	node->left = node->right = NULL;
	node->par = par;
	node->info = info;
	return node;
}

void direct_node(Node* node){
	node->next = next_node(node);
	if(node->next) node->next->prev = node;
	node->prev = prev_node(node);
	if(node->prev) node->prev->next = node;
}

int insert_node(Node** root, char* key, Info info){
	if(!*root){
		*root = create_node(NULL, key, info);
		direct_node(*root);

		return 0;
	}
	Node* node = *root;
	int status;
	int counter = 1;
	while(node){
		status = strcmp(key, node->key);
		if(status > 0){
			if(node->right){
				node = node->right;
				counter++;
			}else{
				node->right = create_node(node, key, info);
				direct_node(node->right);

				return counter;
			}
		}else if(status < 0){
			if(node->left){
				node = node->left;
				counter++;
			}else{
				node->left = create_node(node, key, info);
				direct_node(node->left);

				return counter;
			}
		}else{
			return -1;
		}
	}
}

int size_node(Node* node){
	if(!node){
		return 0;
	}else{
		return size_node(node->left) + size_node(node->right) + 1;
	}
}

Node* find_scapegoat(Node* node, float alfa){
	int size = 1, par_size;
	int height = 1;
	while(node->par){
		par_size = node == node->par->left ? 1 + size + size_node(node->par->right) : 1 + size + size_node(node->par->left);
		if(height > log_alfa(alfa, par_size)){
			return node->par;
		}
		height++;
		size = par_size;
		node = node->par;
	}
}

Node* tree_to_list(Node* node){
	Node* list1 = (node->left) ? tree_to_list(node->left) : NULL;
    Node* list2 = (node->right) ? tree_to_list(node->right) : NULL;
    Node* list3 = node;

    list3->right = list2;
    if (!list1) 
    	return list3;
    Node* last = list1;
    while(last->right) {
    	last=last->right;
    }
    last->right = list3;
    return list1;
}

void ord_list(Node* head){
	Node* par = NULL;
	while(head){
		head->par = par;
		head->left = NULL;

		par = head;
		head = head = head->right;
	}
}

Node* node_from_list(Node* head, int index){
	for(int i = 0; i < index; i++, head = head->right){}
	return head;
}

Node* rebuild_tree(Node* head, Node* par, int size){
	if(size <= 0){
		return NULL;
	}

	Node* sub_root = node_from_list(head, size / 2);
	sub_root->left = rebuild_tree(head, sub_root, size / 2);
	sub_root->right = rebuild_tree(sub_root->right, sub_root, size - size / 2 - 1);
	sub_root->par = par;
}

void insert_tree(Node** root, int* max_size, float alfa, char* key, Info info){
	int height = insert_node(root, key, info);
	if(height == -1){
		return;
	}
	if(height > log_alfa(alfa, size_node(*root))){ // log(1/alfa, size(tree))
		Node* start = find_node(*root, key);
		Node* scapegoat = find_scapegoat(start, alfa);
		int goat_size = size_node(scapegoat);
		Node* head = tree_to_list(scapegoat);
		Node* ptr = head;

		Node* goat_par = scapegoat->par;
		if(goat_par){
			if(scapegoat == goat_par->left){
				goat_par->left = rebuild_tree(head, goat_par, goat_size);
			}else{
				goat_par->right = rebuild_tree(head, goat_par, goat_size);
			}
		}else{
			*root = rebuild_tree(head, NULL, goat_size);
		}

		*max_size++;	
	}
}

const Info* find_min_tree(Node* node){
	Node* find = find_min_node(node);
	if(!find) return NULL;
	return (const Info*)&find->info;
}

const Info* find_max_tree(Node* node){
	Node* find = find_max_node(node);
	if(!find) return NULL;
	return (const Info*)&find->info;
}

const Info* find_tree(Node* root, char* key){
	Node* find = find_node(root, key);
	if(!find) return NULL;
	return (const Info*)&find->info;
}

void clear_node(Node* node){
	if(node){
		free(node->key);
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
		printf("%u, %u\n", node->info.string_num, node->info.offset);
		node = node->next;
	}
}

void print_tree(Node* root){
	Node* start = find_min_node(root);
	print_node(start);
}

int isSymbol(char token){
	return 'a' <= token && token <= 'z' || 'A' <= token && token <= 'Z';
}

void inserter(Node** root, int* max_size, float alfa, int counter, FILE* file, int pad, Info info){
	
	char* key = (char*)malloc(counter + 1);
	fseek(file, -counter - pad, SEEK_CUR);
	fread(key, sizeof(char), counter, file);
	key[counter] = '\0';
	for(int j = 0; j < counter; j++){
		key[j] = ('a' <= key[j] && key[j] <= 'z') * key[j] + ('A' <= key[j] && key[j] <= 'Z') * (key[j] - 'A' + 'a');
	}
	info.offset -= counter + pad;
	insert_tree(root, max_size, alfa, key, info);
	fseek(file, 1, SEEK_CUR);
	
	free(key);
}
//LRU LFU MRU

char* get_str_from_file(FILE* file){
	char buf[80] = {0};
	char* res = NULL;
	int len = 0;
	int n = 0;
	do{
		n = fscanf(file, "%80[^\n]",buf);
		if(n < 0){
			if(!res) return NULL;	
		} else if(n > 0){
			int chunk_len = strlen(buf);
			int str_len = len + chunk_len;
			res = realloc(res,str_len + 1);
			memcpy(res + len, buf, chunk_len);
			len = str_len;
		} else{
			fscanf(file, "%*c");
		}
	}while(n > 0);

	if(len > 0) res[len] = '\0';
	else res = calloc(1,sizeof(char));
	return res;
}

void parser(FILE* file, Node** root, int* max_size, float alfa){
	char* cur_string;
	char* word;
	int row = 0;
	char separators[33] = " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	separators[33] = '\0';
	do{
		cur_string = get_str_from_file(file);
		if(cur_string){
			row++;
			word = strtok(cur_string, separators);
			while (word != NULL){
				for(int i = 0; i < strlen(word); i++){
					word[i] = tolower(word[i]);
				}
				Info info = {row, word - cur_string + 1};
				insert_tree(root, max_size, alfa, word, info);
				word = strtok(NULL, separators);
			}
			free(cur_string);
		}
	}while(cur_string);
}

/*
void parser(FILE* file, Node** root, int* max_size, float alfa){
	int word = 0;
	int counter;
	char token;
	Info info = {0};
	while((token = getc(file)) != EOF){
		info.offset++;
		if(isSymbol(token) && !word){
			word = 1;
			counter = 1;
		}else if(isSymbol(token) && word){
			counter++;
		}else{
			inserter(root, max_size, alfa, counter, file, &word, 1, info);
		}
		if(token == '\n'){
			info.string_num++;
			info.offset = 0;
		}
	}
	inserter(root, max_size, alfa, counter, file, &word, 0, info);
}
*/

int D_fast_search(Node* root){
	char* key1 = read_str("key1");

	if(!key1)
		return -1;

	const Info* info = find_tree(root, key1);
	if(info){
		printf("%u %u\n", info->string_num, info->offset);
	}else{
		printf("nothing\n");
	}

	free(key1);

	return 0;
}

int main(int argc, char const *argv[])
{
	Node* root = NULL;
	int max_size = 0;
	float alfa = 0.5;

	//FILE* file = fopen("test.txt", "r");
	FILE* file = fopen(argv[1], "r");
	if(!file){
		perror("fopen()");
		return EXIT_SUCCESS;
	}

	parser(file, &root, &max_size, alfa);
	
	int done;
	for(;;){
		done = D_fast_search(root);
		if(done < 0)
			break;
	}

	clear_tree(&root);
	fclose(file);
	return 0;
}

/*
При помощи реализованной таблицы (дерева) написать программу для быстрого поиска слова в текстовом файле. Результатом поиска является таблица с расположением
первого вхождения заданного слова в файле (если ключи уникальны). Расположение слова представляет из себя совокупность номера 
строки и смещения слова в этой строке (для текстовых файлов) .
Для выполнения данного пункта задания разрешается изменить тип хранимых в таблице данных (Info).*/