#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

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
	if(ok < 0) return -1;
	while(!ok || !isPrime(answ)){
		flush();
		error();
		printf("Input msize(prime number): ");
		ok = scanf("%d", &answ);
		if(ok < 0) return -1;
	}
	flush();
	printf("\n");
	return answ;
}

int* read_int(char* s){
	int ok;
	int* answ = (int*)malloc(sizeof(int));
	printf("Input %s: ", s);
	ok = scanf("%d", answ);
	if(ok < 0){
		free(answ);
		return NULL;
	}
	while(!ok){
		flush();
		error();
		printf("Input %s: ", s);
		ok = scanf("%d", answ);
		if(ok < 0){
			free(answ);
			return NULL;
		}
	}
	flush();
	printf("\n");
	return answ;
}

char* read_str(char* s){
	char* str;
	printf("\033[1;32mInput %s: \033[0m", s);
	str = get_str();
	if(!str) return NULL;
	printf("\n");
	while(!strlen(str)){
		error();
		printf("\033[1;32mInput %s: \033[0m",s);
		free(str);
		str = get_str();
		if(!str) return NULL;
		printf("\n");
	}
	return str;
}