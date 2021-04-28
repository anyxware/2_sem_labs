#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "b_tree.h"
#include "task.h"
//#define alfa 0.5

int D_Insert(Tree* tree){
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

	Info read_info = insert_tree(tree, key1_1, info);
	if(read_info.string){
		printf("%d %d %s\n", read_info.x, read_info.y, read_info.string);
		free(read_info.string);
	}

	free(info.string);
	free(key1);
	return 0;
}

int D_Find(Tree* tree){
	if(!tree->root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}

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

		const Info* info = find_tree(tree, key1_1);
		if(info){
			printf("%d %d %s\n", info->x, info->y, info->string);
		}

		free(key1);

	}else if(status == 2){
		const Info* info = find_max_tree(tree->root);
		if(info){
			printf("%d %d %s\n", info->x, info->y, info->string);
		}
	}else{
		const Info* info = find_max_tree(tree->root);
		if(info){
			printf("%d %d %s\n", info->x, info->y, info->string);
		}
	}

	return 0;
}

int D_Delete(Tree* tree){
	if(!tree->root){
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

	remove_tree(tree, key1_1);

	free(key1);
	return 0;
}

int D_Show(Tree* tree){
	if(!tree->root){
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
		print_tree(tree->root);
	}else if(status == 2){
		format_print_tree(tree->root);
	}else if(status == 3){
		dot_print_tree(tree->root);
		system("dot ntree.dot | neato -n -Tpng -o ntree.png; gimp ntree.png");
	}
	return 0;
}

int D_Clear(Tree* tree){
	if(!tree->root){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}

	clear_tree(tree);
	return 0;
}

int D_Write(Tree* tree){
	write_tree(*tree);
}

void deleteTree(Tree* tree){
	if(tree->root) clear_tree(tree);
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
	system("clear");

	Tree tree = {0};
	
	/*
	Info info = {1,2,"qwe"};
	insert_tree(&tree, "1", info);
    insert_tree(&tree, "2", info);
    insert_tree(&tree, "8", info);
    insert_tree(&tree, "9", info);
    insert_tree(&tree, "7", info);
    insert_tree(&tree, "3", info);
    insert_tree(&tree,"4", info);
   */

	printf("Read tree from file(Y/n\\{\\n})?\n");
	char* answer = read_str("answer");
	
	if(!answer) return 0;
	else if(answer[0] != 'n') read_tree(&tree);
	else tree.alfa = 0.5;

	free(answer);
	
	int answ;
	int done;

	int (*funcs[7])(Tree* tree) = {NULL, &D_Insert, &D_Find, &D_Delete, &D_Show, &D_Clear, &D_Write};

	while(answ = dialogue()){
		if(funcs[answ]){
			done = funcs[answ](&tree);//funcs[answ](&table);
			if(done < 0)
				break;
			/*
			for(int j = 0; j < BUF_SIZE; j++){
				if(tree.bufer.array[j].busy == 1){
					printf("%s  -  ", tree.bufer.array[j].node->key);
				}else printf("0  -  ");
			}
			printf("\n");
			*/
		}
	}

	//free(tree.bufer);
	deleteTree(&tree);
	return 0;
}