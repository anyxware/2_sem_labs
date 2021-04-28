#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "b_tree.h"

#define lru

unsigned int hash(char key[N]){ //HashFAQ6
	char* str = key;

    unsigned int hash = 0;

    for (; *str; str++)
    {
        hash += (unsigned char)(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

int find_index(Bufer* bufer, int index){
	return bufer->array[index].busy != 1 ? index : bufer->array[index + 1].busy != 1 ? index + 1 : bufer->array[index + 2].busy != 1 ? index + 2 : -1;
}

int find_real_index(Bufer* bufer, int index){
	return bufer->array[index].age < bufer->array[index + 1].age && bufer->array[index].age < bufer->array[index + 2].age ? index :
	bufer->array[index + 1].age < bufer->array[index].age && bufer->array[index + 1].age < bufer->array[index + 2].age ? index + 1 : index + 2;

}

void insert_bufer(Bufer* bufer, Node* node){
	int index = 3/* 3 - bucket size */ * (hash(node->key) % BUF_SIZE); //bucket number, 
	int real_index;
	real_index = find_index(bufer, index); //find free position(index || index + 1 || index + 2)(if no return -1)
	if(real_index == -1){
		real_index = find_real_index(bufer, index); //find the eldest position 
	}
	//inserting
	bufer->max_age++;
	bufer->array[real_index].busy = 1;
	bufer->array[real_index].node = node;
	bufer->array[real_index].age = bufer->max_age;
	return;
}

Node* find_bufer(Bufer* bufer, char key[N]){
	int index = 3 * (hash(key) % BUF_SIZE);
	if(bufer->array[index].busy && !strcmp(bufer->array[index].node->key, key)){
		return bufer->array[index].node;
	}else if(bufer->array[index].busy && !strcmp(bufer->array[index + 1].node->key, key)){
		return bufer->array[index + 1].node;
	}else if(bufer->array[index].busy && !strcmp(bufer->array[index + 2].node->key, key)){
		return bufer->array[index + 2].node;
	}
	return NULL;
}

void remove_bufer(Bufer* bufer, char key[N]){
	int index = 3 * (hash(key) % BUF_SIZE);
	if(bufer->array[index].busy && !strcmp(bufer->array[index].node->key, key)){
		bufer->array[index].busy = 0;
	}else if(bufer->array[index].busy && !strcmp(bufer->array[index + 1].node->key, key)){
		bufer->array[index].busy = 0;
	}else if(bufer->array[index].busy && !strcmp(bufer->array[index + 2].node->key, key)){
		bufer->array[index].busy = 0;
	}
}

/*
void pop_bufer(Bufer* bufer){
	int index = 0;
	int cur_age = bufer->array[index].age;
	for(int i = 1; i < BUF_SIZE; i++){
		#ifdef lru
			if(bufer->array[i].age < cur_age){
				cur_age = bufer->array[i].age;
				index = i;
			}
		#else
			if(bufer->array[i].age > cur_age){
				cur_age = bufer->array[i].age;
				index = i;
			}
		#endif
	}
	bufer->array[index].busy = -1;
}


void insert_bufer(Bufer* bufer, Node* node){
	int index = hash(node->key) % BUF_SIZE;
	for(int i = 0; i < BUF_SIZE; i++){
		if(bufer->array[(index + i) % BUF_SIZE].busy != 1){
			bufer->max_age++;
			bufer->array[(index + i) % BUF_SIZE].busy = 1;
			bufer->array[(index + i) % BUF_SIZE].node = node;
			bufer->array[(index + i) % BUF_SIZE].age = bufer->max_age;
			return;
		}
	}
}
*/
/*
Node* find_bufer(Bufer* bufer, char key[N]){
	int index = hash(key) % BUF_SIZE;
	for(int i = 0; i < BUF_SIZE; i++){
		if(bufer->array[(index + i) % BUF_SIZE].busy == 0){
			return NULL;
		}else if(bufer->array[(index + i) % BUF_SIZE].busy == 1 && !strcmp(bufer->array[(index + i) % BUF_SIZE].node->key, key)){
			bufer->max_age++;
			bufer->array[(index + i) % BUF_SIZE].age = bufer->max_age;
			return bufer->array[(index + i) % BUF_SIZE].node;
		}
	}
	return NULL;
}
*/
/*
void remove_bufer(Bufer* bufer, char key[N]){
	int index = hash(key) % BUF_SIZE;
	for(int i = 0; i < BUF_SIZE; i++){
		if(bufer->array[(index + i) % BUF_SIZE].busy == 0){
			return;
		}else if(bufer->array[(index + i) % BUF_SIZE].busy == 1 && !strcmp(bufer->array[(index + i) % BUF_SIZE].node->key, key)){
			bufer->array[(index + i) % BUF_SIZE].busy = -1;
			return;
		}
	}
}*/

float log_alfa(float alfa, int x){
	return (logf((float)x) / logf(1 / alfa));
}

Node* find_min_node(Node* node){
	for(;node->left; node = node->left){}
	return node;
}

Node* find_max_node(Node* node){
	for(;node->right; node = node->right){}
	return node;
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

Node* create_node(Node* par, char key[N], Info info){
	Node* node = (Node*)malloc(sizeof(Node));
	strcpy(node->key, key);
	node->left = node->right = NULL;
	node->par = par;

	node->info.x = info.x;
	node->info.y = info.y;
	node->info.string = (char*)calloc(1, strlen(info.string) + 1);
	strcpy(node->info.string, info.string);

	return node;
}

void direct_node(Node* node){
	node->next = next_node(node);
	if(node->next) node->next->prev = node;
	node->prev = prev_node(node);
	if(node->prev) node->prev->next = node;
}

RetInfo insert_node(Node** root, char key[N], Info info){
	if(!*root){
		*root = create_node(NULL, key, info);
		direct_node(*root);

		RetInfo retinfo;
		retinfo.height = 0;
		retinfo.new_node = *root;
		return retinfo;
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

				RetInfo retinfo;
				//retinfo.info = NULL;
				retinfo.height = counter;
				retinfo.new_node = node->right;
				return retinfo;
			}
		}else if(status < 0){
			if(node->left){
				node = node->left;
				counter++;
			}else{
				node->left = create_node(node, key, info);
				direct_node(node->left);

				RetInfo retinfo;
				//retinfo.info = NULL;
				retinfo.height = counter;
				retinfo.new_node = node->left;
				return retinfo;
			}
		}else{
			RetInfo retinfo;
			retinfo.info.x = node->info.x;
			retinfo.info.y = node->info.y;
			retinfo.info.string = (char*)calloc(1, strlen(node->info.string) + 1);
			strcpy(retinfo.info.string, node->info.string);
			retinfo.height = -1;

			free(node->info.string);

			node->info.x = info.x;
			node->info.y = info.y;
			node->info.string = (char*)calloc(1, strlen(info.string) + 1);
			strcpy(node->info.string, info.string);

			return retinfo;
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

Node* find_scapegoat(float alfa, Node* node){
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

Info insert_tree(Tree* tree, char key[N], Info info){

	RetInfo retinfo = insert_node(&tree->root, key, info);
	if(retinfo.height == -1){
		return retinfo.info;
	}
	insert_bufer(&tree->bufer, retinfo.new_node);

	if(retinfo.height > log_alfa(tree->alfa, size_node(tree->root))){ // log(1/alfa, size(tree))
		Node* start = find_node(tree->root, key);
		Node* scapegoat = find_scapegoat(tree->alfa, start);
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
			tree->root = rebuild_tree(head, NULL, goat_size);
		}

		tree->max_size++;	
	}
	Info null_info = {0};
	return null_info;
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

const Info* find_tree(Tree* tree, char key[N]){
	Node* buf_find = find_bufer(&tree->bufer, key);
	if(buf_find){
		return (const Info*)&buf_find->info;
	}

	Node* find = find_node(tree->root, key);
	if(!find) return NULL;

	insert_bufer(&tree->bufer, find);

	return (const Info*)&find->info;
}

void remove_node(Node** element, Node* target){
	Node* node = *element;

	if(node->left && node->right){
		Node* max = find_max_node(node->left);

		strcpy(node->key, max->key);
		node->info.x = max->info.x;
		node->info.y = max->info.y;
		strcpy(node->info.string, max->info.string);

		if(node->prev) node->prev->next = node->next;
		if(node->next) node->next->prev = node->prev;

		node->next = max->next;
		node->prev = max->prev;

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
	if(target){
		if(node->prev) node->prev->next = target;
		if(node->next) node->next->prev = target;
	}else{
		if(node->prev) node->prev->next = node->next;
		if(node->next) node->next->prev = node->prev;
	}

	free(node->info.string);
	free(node);
}

//void remove_tree(Node** root, int* max_size, float alfa, char key[N]){
void remove_tree(Tree* tree, char key[N]){
	Node* buf_find = find_bufer(&tree->bufer, key);
	Node* node;

	if(buf_find){
		node = buf_find;
		remove_bufer(&tree->bufer, key);
	}else{
		node = find_node(tree->root, key);
	}

	if(!node) return;
	if(node == tree->root) remove_node(&tree->root, NULL);
	else remove_node(&node, NULL);

	int root_size = size_node(tree->root);
	if(root_size < tree->alfa * tree->max_size){
		Node* head = tree_to_list(tree->root);
		Node* ptr = head;

		tree->root = rebuild_tree(head, NULL, root_size);
		tree->max_size = root_size;
	}
}

void clear_node(Node* node){
	if(node){
		free(node->info.string);
		clear_node(node->left);
		clear_node(node->right);
		free(node);
	}
}

void clear_tree(Tree* tree){
	clear_node(tree->root);
	tree->root = NULL;
	memset(&tree->bufer, 0, sizeof(tree->bufer));
}

void print_node(Node* node){
	while(node){
		printf("%s: ", node->key);
		printf("%d, %d, %s\n", node->info.x, node->info.y, node->info.string);
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
		printf("%d, %d, %s\n", node->info.x, node->info.y, node->info.string);
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
		int len = strlen(node->info.string) + 1;
		fwrite(&len, sizeof(int), 1, file);

		fwrite(node->key, sizeof(char), strlen(node->key), file);
		char c = 0;
		fwrite(&c, sizeof(char), N - strlen(node->key), file);

		fwrite(&node->info.x, sizeof(int), 1, file);
		fwrite(&node->info.y, sizeof(int), 1, file);
		fwrite(node->info.string, sizeof(char), len, file);

		write_node(node->left, file);
		write_node(node->right, file);
	}
}

void write_tree(Tree tree){
	FILE* file = fopen("b_tree.bin", "wb");
	fwrite(&tree.max_size, sizeof(int), 1, file);
	fwrite(&tree.alfa, sizeof(float), 1, file);
	write_node(tree.root, file);
	int end_len = 0;
	fwrite(&end_len, sizeof(int), 1, file);
	fclose(file);
}

void read_tree(Tree* tree/*Node** root, int* max_size, float* alfa*/){
	FILE* file = fopen("b_tree.bin", "rb");
	int len;
	char key[N];
	Info info;
	fread(&tree->max_size, sizeof(int), 1, file);
	fread(&tree->alfa, sizeof(float), 1, file);
	for(;;){
		fread(&len, sizeof(int), 1, file);
		if(!len){
			break;
		}
		fread(key, sizeof(char), N, file);
		fread(&info.x, sizeof(int), 1, file);
		fread(&info.y, sizeof(int), 1, file);
		info.string = (char*)malloc(len);
		fread(info.string, sizeof(char), len, file);
		insert_tree(tree, key, info);
		free(info.string);
	}
	fclose(file);
}

/*
int main(int argc, char const *argv[])
{
	Node* root = NULL;

	Info info1 = {1, 2, "ertftyguh"};
	insert_tree(&root, "qwe", info1);
	insert_tree(&root, "qwer", info1);
	insert_tree(&root, "qw", info1);

	//remove_tree(&root, "qwe");
	format_print_tree(root);	

	return 0;
}
*/

/*
При помощи реализованной таблицы (дерева) написать программу для быстрого поиска слова в текстовом файле. Результатом поиска является таблица с расположением
первого вхождения заданного слова в файле (если ключи уникальны). Расположение слова представляет из себя совокупность номера 
строки и смещения слова в этой строке (для текстовых файлов) .
Для выполнения данного пункта задания разрешается изменить тип хранимых в таблице данных (Info).*/