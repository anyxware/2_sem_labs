#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lab6.c"
#include "table.h"
#include "task.h"

int D_Create(Table** table){
	if(*table){
		printf("\033[1;31mTable has already created\033[0m\n");
		return 0;
	}
	int msize = read_msize();
	if(msize < 0) return -1;
	*table = create_table(msize);
	printf("Table was created with size %d\n", msize);
	return 0;
}

int D_Insert(Table** table){
	char* key1;
	int* check;
	int key2;
	Info info;
	int status;

	if(!*table){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}
	key1 = read_str("key1");

	if(!key1)
		return -1;

	check = read_int("\033[1;33mkey2\033[0m");

	if(check){
		key2 = *check;
		free(check);
	}
	else{
		free(key1);
		return -1;
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

	status = insert_table(*table, key1, key2, info);
	if(status < 0) printf("\033[1;31mDidn't insert\033[0m\n");
	free(key1);
	free(info.string);
	return 0;
}

int D_Find(Table** table){
	char* key1;
	int* check;
	int key2;
	int status, rel;

	if(!*table){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}	
	
	printf("1.Use two keys\n");
	printf("2.Use one key\n");
	printf("3.Use first key and key's release\n");
	status = read_answer(3);
	if(!status) return -1;
	if(status == 1){
		printf("1.Use binary search\n");
		printf("2.Use hash search\n");
		status = read_answer(2);
		if(!status) return -1;
		key1 = read_str("key1");

		if(!key1)
			return -1;

		check = read_int("key2");

		if(check){
			key2 = *check;
			free(check);
		}
		else{
			free(key1);
			return -1;
		}

		if(status == 1){
			const Info* info = KS1_1_search_table(*table, key1, key2);
			if(info){
				printf("%d %d %s\n", info->x, info->y, info->string);
			}
			else{
				printf("Nothing\n");
			}
		}
		else if(status == 2){
			const Info* info = KS2_1_search_table(*table, key1, key2);
			if(info){
				printf("%d %d %s\n", info->x, info->y, info->string);
			}
			else{
				printf("Nothing\n");
			}
		}
		free(key1);
	}
	else if(status == 2){
		printf("1.Use first keys\n");
		printf("2.Use second key\n");
		status = read_answer(2);
		if(!status) return -1;
		if(status == 1){
			key1 = read_str("key1");

			if(!key1)
				return -1;

			InfoR* infor = KS1_2_search_table(*table, key1);
			if(infor){
				for(int i = 0 ;infor[i].info.string; i++){
					printf("%d %d %d %s\n", infor[i].release, infor[i].info.x, infor[i].info.y, infor[i].info.string);
				}
				free(infor);
			}
			else{
				printf("Nothing\n");
			}
			free(key1);
		}
		else if(status == 2){
			check = read_int("key2");

			if(check){
				key2 = *check;
				free(check);
			}
			else{
				return -1;
			}

			const Info* info = KS2_2_search_table(*table, key2);
			if(info){
				printf("%d %d %s\n", info->x, info->y, info->string);
			}
			else{
				printf("Nothing\n");
			}
		}
	}
	else if(status == 3){
		key1 = read_str("key1");

		if(!key1)
			return -1;

		check = read_int("release");

		if(check){
			rel = *check;
			free(check);
		}
		else{
			free(key1);
			return -1;
		}

		const Info* info = search_releases_table(*table, key1, rel);
		if(info){
			printf("%d %d %s\n", info->x, info->y, info->string);
		}
		else{
			printf("Nothing\n");
		}
		free(key1);
	}
	return 0;
}

int D_Delete(Table** table){
	char* key1;
	int* check;
	int key2;
	int status, rel;

	if(!*table){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}
	printf("1.Use two keys\n");
	printf("2.Use one key\n");
	printf("3.Use first key and key's release\n");
	status = read_answer(3);
	if(!status) return -1;
	if(status == 1){
		printf("1.Use binary search\n");
		printf("2.Use hash search\n");
		status = read_answer(2);
		if(!status) return -1;
		key1 = read_str("key1");

		if(!key1)
			return -1;

		check = read_int("key2");

		if(check){
			key2 = *check;
			free(check);
		}
		else{
			free(key1);
			return -1;
		}

		if(status == 1){
			KS1_1_delete_table(*table, key1, key2);
		}
		else if(status == 2){
			KS2_1_delete_table(*table, key1, key2);
		}
		free(key1);
	}
	else if(status == 2){
		printf("1.Use first keys\n");
		printf("2.Use second key\n");
		status = read_answer(2);
		if(!status) return -1;
		if(status == 1){
			key1 = read_str("key1");

			if(!key1)
				return -1;

			KS1_2_delete_table(*table, key1);
			free(key1);
		}
		else if(status == 2){
			check = read_int("key2");

			if(check){
				key2 = *check;
				free(check);
			}
			else{
				return -1;
			}

			KS2_2_delete_table(*table, key2);
		}
	}
	else if(status == 3){
		key1 = read_str("key1");

		if(!key1)
			return -1;

		check = read_int("release");

		if(check){
			rel = *check;
			free(check);
		}
		else{
			free(key1);
			return -1;
		}

		delete_releases_table(*table, key1, rel);
		free(key1);
	}
	return 0;
}

int D_Show(Table** table){
	if(!*table){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}
	show_table(*table);
	return 0;
}

int D_Clear(Table** table){
	if(!*table){
		printf("\033[1;31mNo table\033[0m\n");
		return 0;
	}
	clear_table(*table);
	*table = NULL;
	return 0;
}

int D_Game(Table** table){
	game();
	return 0;
}

void deleteTable(Table** table){
	if(*table) clear_table(*table);
}

int dialogue(){
	printf("\033[1;33m--------MENU---------\033[0m\n");	
	printf("\033[3;44;33m\n");
	printf("1.Create table\n");
	printf("2.Insert info\n");
	printf("3.Find info\n");
	printf("4.Delete info\n");
	printf("5.Show table\n");
	printf("6.Remove table\n");
	printf("7.Play game\033[0m\n");
	printf("(Ctrl+D if you want to exit)\n\n");
	return read_answer(8);
}

int main(int argc, char const *argv[])
{
	Table *table = NULL;

	int answ;
	int done;

	int (*funcs[8])(Table** table) = {NULL, &D_Create, &D_Insert, &D_Find, &D_Delete, &D_Show, &D_Clear, &D_Game};

	while(answ = dialogue()){
		if(funcs[answ]){
			done = funcs[answ](&table);//funcs[answ](&table);
			if(done < 0)
				break;
		}
	}

	deleteTable(&table);
	return 0;
}