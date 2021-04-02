#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "lab6.c"
#include "nwtable.h"
#include "task.h"

int D_Create(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	int msize = read_msize();
	if(msize < 0) return -1;
	create_wtable(table, ks1, ks2, file, msize);
	printf("Table was created with size %d\n", msize);
	return 0;
}

int D_Insert(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	char* key1;
	int* check;
	int key2;
	InfoS infos;
	int status;

	key1 = read_str("key1");

	if(!key1)
		return -1;

	char key1_1[N] = {'0'};
	for (int i = 0; i < N && i < strlen(key1); ++i){
		key1_1[i] = key1[i];
	}

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
		infos.info.x = *check;
		free(check);
	}
	else{
		free(key1);
		return -1;
	}

	check = read_int("\033[1;35my\033[0m");

	if(check){
		infos.info.y = *check;
		free(check);
	}
	else{
		free(key1);
		return -1;
	}

	infos.string = read_str("string");

	if(!infos.string){
		free(key1);
		return -1;
	}

	status = insert_wtable(table, ks1, ks2, file, key1_1, key2, infos);
	if(status < 0) printf("\033[1;31mDidn't insert\033[0m\n");
	free(key1);
	free(infos.string);
	return 0;
}

int D_Find(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	char* key1;
	int* check;
	int key2;
	int status, rel;	
	static int iterator = 0;
	
	printf("1.Use two keys\n");
	printf("2.Use one key\n");
	printf("3.Use first key and key's release\n");
	status = read_answer(3);
	if(status == 1){
		printf("1.Use binary search\n");
		printf("2.Use hash search\n");
		status = read_answer(2);
		key1 = read_str("key1");

		if(!key1)
			return -1;

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

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
			InfoS* infos = KS1_1_search_wtable(table, ks1, ks2, file, key1_1, key2);
			if(infos){
				printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
				free(infos->string);
				free(infos);
			}
			else{
				printf("Nothing\n");
			}
		}
		else if(status == 2){
			InfoS* infos = KS2_1_search_wtable(table, ks1, ks2, file, key1_1, key2);
			if(infos){
				printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
				free(infos->string);
				free(infos);
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
		if(status == 1){
			key1 = read_str("key1");

			if(!key1)
				return -1;

			char key1_1[N] = {'0'};
			for (int i = 0; i < N && i < strlen(key1); ++i){
				key1_1[i] = key1[i];
			}

			InfoSR* infosr = KS1_2_search_wtable(table, ks1, ks2, file, key1_1);
			if(infosr){
				//printf("%d\n", infosr[0].info.len);
				int ind = 1 + iterator % (infosr[0].info.len - 1);
				printf("%d %d %d %s\n", infosr[ind].release, infosr[ind].info.x, infosr[ind].info.y, infosr[ind].string);
				for(int i = 1;i < infosr[0].info.len; i++){
					//printf("%d %d %d %s\n", infosr[i].release, infosr[i].info.x, infosr[i].info.y, infosr[i].string);
					free(infosr[i].string);
				}
				//printf("%d\n", iterator);
				iterator++;
				free(infosr);
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
			InfoS* infos = KS2_2_search_wtable(table, ks1, ks2, file, key2);
			if(infos){
				printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
				free(infos->string);
				free(infos);
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

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		check = read_int("release");

		if(check){
			rel = *check;
			free(check);
		}
		else{
			free(key1);
			return -1;
		}
		InfoS* infos = search_releases_wtable(table, ks1, ks2, file, key1_1, rel);
		if(infos){
			printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
			free(infos->string);
			free(infos);
		}
		else{
			printf("Nothing\n");
		}
		free(key1);
	}
	return 0;
}

int D_Delete(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	char* key1;
	int* check;
	int key2;
	int status, rel;

	printf("1.Use two keys\n");
	printf("2.Use one key\n");
	printf("3.Use first key and key's release\n");
	status = read_answer(3);
	if(status == 1){
		printf("1.Use binary search\n");
		printf("2.Use hash search\n");
		status = read_answer(2);
		key1 = read_str("key1");

		if(!key1)
			return -1;

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

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
			KS1_1_delete_wtable(table, ks1, ks2, file, key1_1, key2);
		}
		else if(status == 2){
			KS2_1_delete_wtable(table, ks1, ks2, file, key1_1, key2);
		}
		free(key1);
	}
	else if(status == 2){
		printf("1.Use first keys\n");
		printf("2.Use second key\n");
		status = read_answer(2);
		if(status == 1){
			key1 = read_str("key1");

			if(!key1)
				return -1;

			char key1_1[N] = {'0'};
			for (int i = 0; i < N && i < strlen(key1); ++i){
				key1_1[i] = key1[i];
			}

			KS1_2_delete_wtable(table, ks1, ks2, file, key1_1);
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
			KS2_2_delete_wtable(table, ks1, ks2, file, key2);
		}
	}
	else if(status == 3){
		key1 = read_str("key1");

		if(!key1)
			return -1;

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		check = read_int("release");

		if(check){
			rel = *check;
			free(check);
		}
		else{
			free(key1);
			return -1;
		}
		delete_releases_wtable(table, ks1, ks2, file, key1_1, rel);
		free(key1);
	}
	return 0;
}

int D_Show(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	show_wtable(table, ks1, ks2, file);
	return 0;
}

int D_Clear(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	clear_wtable(table, ks1, ks2, file);
	return 0;
}

int D_GarbageCollector(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	garbage_collector_wtable(table, ks1, ks2, file);
	return 0;
}

int D_Game(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
	rewind(file);
	game();
	return 0;
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
	printf("7.Garbage Collector\n");
	printf("8.Play game\033[0m\n");
	printf("(Ctrl+D if you want to exit)\n\n");
	return read_answer(9);
}

void read1_table(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file){
    fread(table, sizeof(Table), 1, file);

    *ks1 = (KeySpace1*)malloc(table->msize * sizeof(KeySpace1));
	*ks2 = (KeySpace2*)malloc(table->msize * sizeof(KeySpace2));
    fread(*ks1, sizeof(KeySpace1), table->msize, file);
    fread(*ks2, sizeof(KeySpace2), table->msize, file);
}

int main(int argc, char const *argv[])
{
	Table table;
	KeySpace1* ks1 = NULL;
	KeySpace2* ks2 = NULL;

	int empty;

	FILE* file = fopen("table.bin", "r+b");

	if(!file){
		perror("fopen() ");
		return EXIT_SUCCESS;
	}

	fseek(file, 0, SEEK_END);
	if(!ftell(file)){
		printf("File is empty\n");
		empty = 1;
	}else{
		rewind(file);
		read1_table(&table, &ks1, &ks2, file);
		empty = 0;
	}

	int answ;
	int done;
	//int iterator = 0;

	int (*funcs[9])(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file) = {NULL, &D_Create, &D_Insert, &D_Find, &D_Delete, &D_Show, &D_Clear, &D_GarbageCollector, &D_Game};

	while(answ = dialogue()){

		if(funcs[answ]){

			if(answ == 1 && !empty){
				printf("table has already been created\n");
				continue;
			}else if(answ != 1 && empty){
				printf("no table\n");
				continue;
			}

			done = answ[funcs](&table, &ks1, &ks2, file);//funcs[answ](&table);
			if(done < 0)
				break;

			if(answ == 1){
				empty = 0;
			}else if(answ == 6){
				empty = 1;

			}
		}

	}

	//5[funcs](&table, &ks1, &ks2, file);

	if(!empty){
		//printf("test\n");
		rewind(file);
		fwrite(&table, sizeof(Table), 1, file);
		fwrite(ks1, sizeof(KeySpace1), table.msize, file);
		fwrite(ks2, sizeof(KeySpace2), table.msize, file);
	}
	if(ks1) free(ks1);
	if(ks2) free(ks2);
	fclose(file);


	return 0;
}


/*
int main(int argc, char const *argv[])
{

	Table table;
	KeySpace1* ks1 = NULL;
	KeySpace2* ks2 = NULL;

	int empty;

	FILE* file = fopen("table.bin", "r+b");
	fseek(file, 0, SEEK_END);
	if(!ftell(file)){
		printf("File is empty\n");
		empty = 1;
	}else{
		rewind(file);
		read1_table(&table, &ks1, &ks2, file);
		empty = 0;
	}


	int answ;
	int done;

	int (*funcs[9])(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file) = {NULL, &D_Create, &D_Insert, &D_Find, &D_Delete, &D_Show, &D_Clear, &D_GarbageCollector, &D_Game};

	while(answ = dialogue()){


		if(funcs[answ]){

			if(answ == 1 && !empty){
				printf("table has already created\n");
				continue;
			}else if(answ != 1 && empty){
				printf("no table\n");
				continue;
			}

			if(answ == 1){
				empty = 0;
			}else if(answ == 6){
				empty = 1;
			}

			done = answ[funcs](&table, &ks1, &ks2, file);//funcs[answ](&table);
			if(done < 0)
				break;
		}
	}

	if(!empty){
		rewind(file);
		fwrite(&table, sizeof(Table), 1, file);
		fwrite(ks1, sizeof(KeySpace1), table.msize, file);
		fwrite(ks2, sizeof(KeySpace2), table.msize, file);
	}	
		
	if(ks1) free(ks1);
	if(ks2) free(ks2);
	fclose(file);


	return 0;
}*/