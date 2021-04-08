#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ntree.h"
#include "task.h"

int D_Insert(Node** root){
	if(!*root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}

	char* key1;

	key1 = read_str("key1");

	if(!key1)
		return -1;

	char key1_1[N] = {'0'};
	for (int i = 0; i < N && i < strlen(key1); ++i){
		key1_1[i] = key1[i];
	}

	char* string = read_str("string");

	if(!string){
		free(key1);
		return -1;
	}

	insert_tree(root, key1_1, string);
	free(key1);
	free(string);
	return 0;
}

int D_Find(Node** root){
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

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		strings = find_tree(*root, key1_1);

		free(key1);

	}else if(status == 2){
		strings = find_max_tree(*root);
	}else{
		strings = find_min_tree(*root);
	}
	
	if(strings){
		for(int i = 0; strings[i]; i++){
			printf("%s, ", strings[i]);
		}
		free(strings);
	}

	return 0;
}

int D_Delete(Node** root){
	if(!*root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}

	char* key1;
	int status;

	
	key1 = read_str("key1");

	if(!key1)
		return -1;

	char key1_1[N] = {'0'};
	for (int i = 0; i < N && i < strlen(key1); ++i){
		key1_1[i] = key1[i];
	}

	remove_tree(root, key1_1);

	free(key1);
	return 0;
}

int D_Show(Node** root){
	if(!*root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}
	print_tree(*root);
	return 0;
}

int D_Clear(Node** root){
	if(!*root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}

	clear_tree(root);
	return 0;
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
	printf("\033[0m\n");
	printf("(Ctrl+D if you want to exit)\n\n");
	return read_answer(6);
}

int main(int argc, char const *argv[])
{
	Node* root = NULL;

	insert_tree(&root, "1", "1");
    insert_tree(&root, "2", "2");
    insert_tree(&root, "8", "8");
    insert_tree(&root, "9", "9");
    insert_tree(&root, "7", "7");
    insert_tree(&root, "3", "3");
    insert_tree(&root, "4", "4");

	int answ;
	int done;

	int (*funcs[6])(Node** root) = {NULL, &D_Insert, &D_Find, &D_Delete, &D_Show, &D_Clear};

	while(answ = dialogue()){
		if(funcs[answ]){
			done = funcs[answ](&root);//funcs[answ](&table);
			if(done < 0)
				break;
		}
	}

	deleteTree(&root);
	return 0;
}