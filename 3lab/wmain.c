#include "wtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lab6.c"

char* get_str(){
	char buf[80] = {0};
	char* res = NULL;
	int len = 0;
	int n = 0;
	do{
		n = scanf("%80[^\n]",buf);
		if(n < 0){
			if(!res) return NULL;	
		} else if(n > 0){
			int chunk_len = strlen(buf);
			int str_len = len + chunk_len;
			res = realloc(res,str_len + 1);
			memcpy(res + len, buf, chunk_len);
			len = str_len;
		} else{
			scanf("%*c");
		}
	}while(n > 0);

	if(len > 0) res[len] = '\0';
	else res = calloc(1,sizeof(char));
	return res;
}

int isPrime(int n){
	if (n > 1){
		for (int i = 2; i < n; i++)
			if (n % i == 0)
				return 0;
		return 1;
	}else
		return 0;
}

void error(){
	printf("\033[1;31mWrong answer!\033[0m\n");
}

void flush(){
	char c;
	while((c = getchar()) != '\n'){}
}

int read_answer(int bord){
	int ok;
	int answ;
	printf("Input answer: ");
	ok = scanf("%d", &answ);
	if(ok < 0) return 0;
	while(!ok || answ < 1 || answ > bord){
		flush();
		error();
		printf("Input answer: ");
		ok = scanf("%d", &answ);
		if(ok < 0) return 0;
	}
	flush();
	printf("\n");
	return answ;
}

int read_msize(){
	int ok;
	int answ;
	printf("Input msize(prime number): ");
	ok = scanf("%d", &answ);
	while(!ok || !isPrime(answ)){
		flush();
		error();
		printf("Input msize(prime number): ");
		ok = scanf("%d", &answ);
	}
	flush();
	printf("\n");
	return answ;
}

int read_int(char* s){
	int ok;
	int answ;
	printf("Input %s: ", s);
	ok = scanf("%d", &answ);
	while(!ok){
		flush();
		error();
		printf("Input %s: ", s);
		ok = scanf("%d", &answ);
	}
	flush();
	printf("\n");
	return answ;
}

char* read_str(char* s){
	char* str;
	printf("\033[1;32mInput %s: \033[0m", s);
	str = get_str(); printf("\n");
	while(!strlen(str)){
		error();
		printf("\033[1;32mInput %s: \033[0m",s);
		free(str);
		str = get_str();
		printf("\n");
	}

	return str;
}

void D_Create(){
	int msize = read_msize();
	create_wtable(msize);
	printf("Table was created with size %d\n", msize);
}

void D_Insert(){
	char* key1;
	int key2;
	InfoS infos;
	int status;

	key1 = read_str("key1");

	char key1_1[N] = {'0'};
	for (int i = 0; i < N && i < strlen(key1); ++i){
		key1_1[i] = key1[i];
	}

	key2 = read_int("\033[1;33mkey2\033[0m");
	infos.info.x = read_int("\033[1;34mx\033[0m");
	infos.info.y = read_int("\033[1;35my\033[0m");
	infos.string = read_str("string");

	status = insert_wtable(key1_1, key2, infos);
	if(status < 0) printf("\033[1;31mDidn't insert\033[0m\n");
	free(key1);
	free(infos.string);
}

void D_Find(){
	char* key1;
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

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		key2 = read_int("key2");

		if(status == 1){
			InfoS* infos = KS1_1_search_wtable(key1_1, key2);
			if(infos){
				printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
			}
			else{
				printf("Nothing\n");
			}
		}
		else if(status == 2){
			InfoS* infos = KS2_1_search_wtable(key1_1, key2);
			if(infos){
				printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
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

			char key1_1[N] = {'0'};
			for (int i = 0; i < N && i < strlen(key1); ++i){
				key1_1[i] = key1[i];
			}

			InfoSR* infosr = KS1_2_search_wtable(key1_1);
			if(infosr){
				for(int i = 0 ;infosr[i].string; i++){
					printf("%d %d %d %s\n", infosr[i].release, infosr[i].info.x, infosr[i].info.y, infosr[i].string);
				}
				free(infosr);
			}
			else{
				printf("Nothing\n");
			}
			free(key1);
		}
		else if(status == 2){
			key2 = read_int("key2");
			InfoS* infos = KS2_2_search_wtable(key2);
			if(infos){
				printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
			}
			else{
				printf("Nothing\n");
			}
		}
	}
	else if(status == 3){
		key1 = read_str("key1");

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		rel = read_int("release");
		InfoS* infos = search_releases_wtable(key1_1, rel);
		if(infos){
			printf("%d %d %s\n", infos->info.x, infos->info.y, infos->string);
		}
		else{
			printf("Nothing\n");
		}
		free(key1);
	}
}

void D_Delete(){
	char* key1;
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

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		key2 = read_int("key2");
		if(status == 1){
			KS1_1_delete_wtable(key1_1, key2);
		}
		else if(status == 2){
			KS2_1_delete_wtable(key1_1, key2);
		}
		free(key1);
	}
	else if(status == 2){
		printf("1.Use first keys\n");
		printf("2.Use second key\n");
		status = read_answer(2);
		if(status == 1){
			key1 = read_str("key1");

			char key1_1[N] = {'0'};
			for (int i = 0; i < N && i < strlen(key1); ++i){
				key1_1[i] = key1[i];
			}

			KS1_2_delete_wtable(key1_1);
			free(key1);
		}
		else if(status == 2){
			key2 = read_int("key2");
			KS2_2_delete_wtable(key2);
		}
	}
	else if(status == 3){
		key1 = read_str("key1");

		char key1_1[N] = {'0'};
		for (int i = 0; i < N && i < strlen(key1); ++i){
			key1_1[i] = key1[i];
		}

		rel = read_int("release");
		delete_releases_wtable(key1_1, rel);
		free(key1);
	}
}

void D_Show(){
	show_wtable();
}

void D_Clear(){
	clear_wtable();
}

void D_GarbageCollector(){
	garbage_collector_wtable();
}

void D_Game(){
	game();
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

int main(int argc, char const *argv[])
{
	Table *table = NULL;

	int answ;

	void (*funcs[9])() = {NULL, &D_Create, &D_Insert, &D_Find, &D_Delete, &D_Show, &D_Clear, &D_GarbageCollector, &D_Game};

	while(answ = dialogue()){
		if(funcs[answ]){
			funcs[answ](&table);
		}
	}

	return 0;
}