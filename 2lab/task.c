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

int oprndcmp(char token, char element){
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

int symbol(char token){
	return token >= 'A' && token <= 'Z' || token >= 'a' && token <= 'z';
}

int operand(char token){
	return token == '*' || token == '/' || token == '+' || token == '-';
}

int check_token(char* infix, char* p){
	if(p == infix && operand(*p)) return 1;
	if(symbol(*p) && *(p+1) == '(') return 1; // a(
	if(*p == ')' && symbol(*(p+1))) return 1; // )a
	if(*p == '(' && operand(*(p+1))) return 1; // (+
	if(operand(*p) && *(p+1) == ')') return 1; // +)
	if(symbol(*p) && symbol(*(p+1))) return 1; // aa
	if(operand(*p) && operand(*(p+1))) return 1; // ++
	return 0;
}

int infix_to_postfix(char* infix, char* postfix){
	STACK Stack;
	stack_init(&Stack, 4, sizeof(char));
	int j = 0;
	int bracket_flag = 0, err_flag = 0, push_flag = 1;
	char token, element;
	void* status;
	for(char* p = infix; *p != '\0'; p++){
		token = *p;
		if(check_token(infix, p)) {
			err_flag = 1;
			break;
		}
		if(symbol(token)){
			postfix[j++] = token;
		}
		else if(token == '('){
			push_flag = stack_push(&Stack, (void*)&token);
			bracket_flag = 1;
		}
		else if(token == ')' && bracket_flag){
			do{
				status = stack_pop(&Stack);
				element = status ? *((char*)status) : 0;
				free(status);
				if(element != '(') postfix[j++] = element;
			}while(element != '(');
			bracket_flag = 0;
		}
		else if(operand(token)){
			do{
				status = stack_pop(&Stack);
				element = status ? *((char*)status) : 0;
				free(status);
				if(oprndcmp(token, element)){
					postfix[j++] = element;
				}
				else{
					push_flag = stack_push(&Stack, (void*)&element);
					break;
				}
			}while(1);
			push_flag = stack_push(&Stack, (void*)&token);
		}
		else{
			err_flag = 2;
			break;
		}
	}
	do{
		status = stack_pop(&Stack);
		element = status ? *((char*)status) : 0;
		free(status);
		if(element) postfix[j++] = element;
		else break;
	}while(1);
	stack_clear(&Stack);
	if(bracket_flag) err_flag = 1;
	if(!push_flag) err_flag = 3;
	switch(err_flag){
		case 1:
		printf("\033[1;31mWrong entry!\033[0m");
		break;
		case 2:
		printf("\033[1;31mWrong symbols!\033[0m");
		break;
		case 3:
		printf("\033[1;31mStackoverflow!\033[0m");
		break;
	}
	if(err_flag) return 0;
	return j;
}