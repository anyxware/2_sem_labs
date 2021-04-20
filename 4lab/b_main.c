#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "b_tree.h"
#include "task.h"

int D_Insert(Node** root, int* max_size){
	Info info;
	int* check;
	char* key1;

	key1 = read_str("key1");

	if(!key1)
		return -1;

	char key1_1[N] = {0};
	for (int i = 0; i < N && i < strlen(key1); ++i){
		key1_1[i] = key1[i];
	}

	check = read_int("\033[1;34mx\033[0m");

	if(check){
		info.x = *check;
		free(check);
	}
	else{
		free(key1);
		return -1;
	}

	check = read_int("\033[1;35my\033[0m");

	if(check){
		info.y = *check;
		free(check);
	}
	else{
		free(key1);
		return -1;
	}

	info.string = read_str("string");

	if(!info.string){
		free(key1);
		return -1;
	}

	const Info* read_info = insert_tree(root, max_size, key1_1, info);
	if(read_info){
		printf("%d %d %s\n", read_info->x, read_info->y, read_info->string);
	}

	/*
	RetInfo retinfo = insert_tree(root, key1_1, info);
	if(retinfo.info){
		printf("%d %d %s\n", retinfo.info->x, retinfo.info->y, retinfo.info->string);
	}else{
		printf("height: %d\n", retinfo.height);
	}
	*/
	free(info.string);
	free(key1);
	return 0;
}

int D_Find(Node** root, int* max_size){
	char* key1;
	int status;
	
	printf("1.Find element by key\n");
	printf("2.Find max element\n");
	printf("3.Find min element\n");
	status = read_answer(3);
	char** strings = NULL;
	if(status == 1){
		key1 = read_str("key1");

		if(!key1)
			return -1;

		char key1_1[N] = {0};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		const Info* info = find_tree(*root, key1_1);
		if(info){
			printf("%d %d %s\n", info->x, info->y, info->string);
		}

		free(key1);

	}else if(status == 2){
		const Info* info = find_max_tree(*root);
		if(info){
			printf("%d %d %s\n", info->x, info->y, info->string);
		}
	}else{
		const Info* info = find_max_tree(*root);
		if(info){
			printf("%d %d %s\n", info->x, info->y, info->string);
		}
	}

	return 0;
}

int D_Delete(Node** root, int* max_size){
	if(!*root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}

	char* key1;
	int status;

	
	key1 = read_str("key1");

	if(!key1)
		return -1;

	char key1_1[N] = {0};
	for (int i = 0; i < N && i < strlen(key1); ++i){
		key1_1[i] = key1[i];
	}

	remove_tree(root, max_size, key1_1);

	free(key1);
	return 0;
}

int D_Show(Node** root, int* max_size){
	if(!*root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}
	printf("1.Direct order\n");
	printf("2.Format print\n");
	printf("3.Graphviz\n");
	int status = read_answer(3);
	if(!status){
		return -1;
	}else if(status == 1){
		print_tree(*root);
	}else if(status == 2){
		format_print_tree(*root);
	}else if(status == 3){
		dot_print_tree(*root);
		system("dot ntree.dot | neato -n -Tpng -o ntree.png; gimp ntree.png");
	}
	return 0;
}

int D_Clear(Node** root, int* max_size){
	if(!*root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}

	clear_tree(root);
	return 0;
}

int D_Write(Node** root, int* max_size){
	write_tree(*root);
}

void deleteTree(Node** root){
	if(*root) clear_tree(root);
}

int dialogue(){
	printf("\033[1;33m--------MENU---------\033[0m\n");	
	printf("\033[3;44;33m\n");
	printf("1.Insert info\n");
	printf("2.Find info\n");
	printf("3.Delete info\n");
	printf("4.Show table\n");
	printf("5.Remove table\n");
	printf("6.Write\n");
	printf("\033[0m\n");
	printf("(Ctrl+D if you want to exit)\n\n");
	return read_answer(6);
}

int main(int argc, char const *argv[])
{
	Node* root = NULL;
	int max_size = 0;

	
	Info info = {1,2,"qwe"};
	insert_tree(&root, &max_size, "1", info);
    insert_tree(&root,&max_size, "2", info);
    insert_tree(&root,&max_size, "8", info);
    insert_tree(&root,&max_size, "9", info);
    insert_tree(&root,&max_size, "7", info);
    insert_tree(&root,&max_size, "3", info);
    insert_tree(&root,&max_size ,"4", info);
    

	printf("Read tree from file(Y/n\\{\\n})?\n");
	char* answer = read_str("answer");
	if(!answer) return 0;
	else if(answer[0] != 'n') read_tree(&root, &max_size);
	free(answer);
	
	int answ;
	int done;

	int (*funcs[7])(Node** root, int* max_size) = {NULL, &D_Insert, &D_Find, &D_Delete, &D_Show, &D_Clear, &D_Write};

	while(answ = dialogue()){
		if(funcs[answ]){
			done = funcs[answ](&root, &max_size);//funcs[answ](&table);
			if(done < 0)
				break;
		}
	}
	/*

	Node* node = find_node(root, "8");
	Node* head = tree_to_list(node);
	while(head){
		printf("%s\n", head->key);
		head = head->right;
	}
	*/

	deleteTree(&root);
	return 0;
}