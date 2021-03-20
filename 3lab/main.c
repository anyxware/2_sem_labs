#include "table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[])
{
	Table *table = NULL;
	int answ;
	int msize;
	char* key1;
	int key2;
	Info info;
	int status;
	int rel;
	while(1){
		printf("\033[1;33m--------MENU---------\033[0m\n");	
		printf("\033[3;44;33m\n");
		printf("1.Create table\n");
		printf("2.Insert info\n");
		printf("3.Find info\n");
		printf("4.Delete info\n");
		printf("5.Show table\n");
		printf("6.Remove table\033[0m\n\n");
		
		answ = read_answer(7);
		//flush();
		if(!answ) break;

		if(answ == 1){
//------
			if(table){
				printf("\033[1;31mTable has already created\033[0m\n");
				continue;
			}
			msize = read_msize();
			table = create_table(msize);
			printf("Table was created with size %d\n", msize);
		}
		else if(answ == 2){
//------
			if(!table){
				printf("\033[1;31mNo table\033[0m\n");
				continue;
			}
			printf("\033[1;32mInput key1: \033[0m");
			key1 = get_str(); printf("\n");
			key2 = read_int("\033[1;33mkey2\033[0m");
			info.x = read_int("\033[1;34mx\033[0m");
			info.y = read_int("\033[1;35my\033[0m");
			printf("\033[1;36mInput string: \033[0m");
			info.string = get_str(); printf("\n");

			status = insert_table(table, key1, key2, info);
			if(status < 0) printf("\033[1;31mDidn't insert\033[0m\n");
			free(key1);
			free(info.string);
		}
		else if(answ == 3){
//------	
			if(!table){
				printf("\033[1;31mNo table\033[0m\n");
				continue;
			}	
			printf("1.Use two keys\n");
			printf("2.Use one key\n");
			printf("3.Use first key and key's release\n");
			status = read_answer(3);
			if(status == 1){
				printf("1.Use binary search\n");
				printf("2.Use hash search\n");
				status = read_answer(2);
				printf("Input key1: ");
				key1 = get_str(); printf("\n");
				key2 = read_int("key2");
				if(status == 1){
					const Info* info = KS1_1_search_table(table, key1, key2);
					if(info){
						printf("%d %d %s\n", info->x, info->y, info->string);
					}
					else{
						printf("Nothing\n");
					}
				}
				else if(status == 2){
					const Info* info = KS2_1_search_table(table, key1, key2);
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
				if(status == 1){
					printf("Input key1: ");
					key1 = get_str(); printf("\n");
					InfoR* infor = KS1_2_search_table(table, key1);
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
					key2 = read_int("key2");
					const Info* info = KS2_2_search_table(table, key2);
					if(info){
						printf("%d %d %s\n", info->x, info->y, info->string);
					}
					else{
						printf("Nothing\n");
					}
				}
			}
			else if(status == 3){
				printf("Input key1: ");
				key1 = get_str(); printf("\n");
				rel = read_int("release");
				const Info* info = search_releases_table(table, key1, rel);
				if(info){
					printf("%d %d %s\n", info->x, info->y, info->string);
				}
				else{
					printf("Nothing\n");
				}
				free(key1);
			}
		}
		else if(answ == 4){
//------
			if(!table){
				printf("\033[1;31mNo table\033[0m\n");
				continue;
			}
			printf("1.Use two keys\n");
			printf("2.Use one key\n");
			printf("3.Use first key and key's release\n");
			status = read_answer(3);
			if(status == 1){
				printf("1.Use binary search\n");
				printf("2.Use hash search\n");
				status = read_answer(2);
				printf("Input key1: ");
				key1 = get_str(); printf("\n");
				key2 = read_int("key2");
				if(status == 1){
					KS1_1_delete_table(table, key1, key2);
				}
				else if(status == 2){
					KS2_1_delete_table(table, key1, key2);
				}
				free(key1);
			}
			else if(status == 2){
				printf("1.Use first keys\n");
				printf("2.Use second key\n");
				status = read_answer(2);
				if(status == 1){
					printf("Input key1: ");
					key1 = get_str(); printf("\n");
					KS1_2_delete_table(table, key1);
					free(key1);
				}
				else if(status == 2){
					key2 = read_int("key2");
					KS2_2_delete_table(table, key2);
				}
			}
			else if(status == 3){
				printf("Input key1: ");
				key1 = get_str(); printf("\n");
				rel = read_int("release");
				delete_releases_table(table, key1, rel);
				free(key1);
			}	
		}
		else if(answ == 5){
//------
			if(!table){
				printf("\033[1;31mNo table\033[0m\n");
				continue;
			}
			show_table(table);
		}
		else if(answ == 6){
			if(!table){
				printf("\033[1;31mNo table\033[0m\n");
				continue;
			}
			clear_table(table);
			table = NULL;
		}
		printf("\n");
	}
	if(table) clear_table(table);
	return 0;
}