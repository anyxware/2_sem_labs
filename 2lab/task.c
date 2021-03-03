#include <string.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "stack.h"
#define SIZE 80

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

void show(char* postfix, int len){
	for(int i = 0; i < len; i++){
		printf("%c", postfix[i]);
	}
	printf("\n");
}

void clean(char* infix, char* postfix){
	free(infix);
	free(postfix);
}

int comprasion(char token, char element){
	if(token == '+' || token == '-'){
		if(element == '+' || element == '-' || element == '*' || element == '/')
			return 1;
		else
			return 0;
	}else if(token == '*' || token == '/'){
		if(element == '*' || element == '/')
			return 1;
		else
			return 0;
	}
}

int check_token(char* infix, int i){
	/*
	if(infix[i+1] == '(' && infix[i] != '*' && infix[i] != '/' && infix[i] != '+' && infix[i] != '-') return 1; // (+
	if(infix[i] == ')' && infix[i+1] != '*' && infix[i+1] != '/' && infix[i+1] != '+' && infix[i+1] != '-' && infix[i+1] != 0) return 2; // +) 
	if(infix[i] == '(' && (infix[i+1] < 'A' || infix[i+1] > 'Z' || infix[i+1] < 'a' || infix[i+1] > 'z' || infix[i+1] == ')' || infix[i+1] == '(') && infix[i+1] != 0) return 3; // a(
	if(infix[i+1] == ')' && (infix[i] < 'A' || infix[i] > 'Z' || infix[i] < 'a' || infix[i] > 'z' || infix[i] == ')' || infix[i] == '(')) return 4; // )a
	if((infix[i] >= 'A' && infix[i] <= 'Z' || infix[i] >= 'a' && infix[i] <= 'z') && (infix[i+1] >= 'A' && infix[i+1] <= 'Z' || infix[i+1] >= 'a' && infix[i+1] <= 'z')) return 5; // aa
	*/
	return 0;
}


int infix_to_postfix(char* infix, char* postfix){
	STACK Stack;
	stack_init(&Stack, strlen(infix), sizeof(char));
	int j = 0;
	int bracket_flag = 0;
	char token, element;
	void* status;
	int y = 0;
	for(int i = 0; i < strlen(infix); i++){
		token = infix[i];
		if(check_token(infix, i)){
			printf("\033[1;31mWrong entry!\033[0m\n");
			printf("%d\n",check_token(infix,i) );
			return 0;
		}

		if(token >= 'A' && token <= 'Z' || token >= 'a' && token <= 'z'){
			postfix[j++] = token;
		}
		else if(token == '('){
			stack_push(&Stack, (void*)&token);//uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
			bracket_flag = 1;
		}
		else if(token == ')' && bracket_flag){
			do{
				status = stack_pop(&Stack);//ooooooooooooooooooooooooooooooooooooooooooo
				element = status ? *((char*)status) : 0;
				free(status);//------
				if(element != '(') postfix[j++] = element;
			}while(element != '(');
			bracket_flag = 0;
		}
		else if(token == '*' || token == '/' || token == '+' || token == '-'){
			do{
				status = stack_pop(&Stack);//ooooooooooooooooooooooooooooooooooo
				element = status ? *((char*)status) : 0;
				free(status);//------
				if(comprasion(token, element)){
					postfix[j++] = element;
				}
				else{
					stack_push(&Stack, (void*)&element);//uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
					break;
				}
			}while(1);
			stack_push(&Stack, (void*)&token);//uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
		}
		else{
			printf("-----\n");
		}
	}
	do{
		status = stack_pop(&Stack);//oooooooooooooooooooooooooooooooooooo
		element = status ? *((char*)status) : 0;
		free(status);//----
		if(element){
			postfix[j++] = element;
		}
		else
			break;
	}while(1);
	stack_clear(&Stack);
	return j;
}