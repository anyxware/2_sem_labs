#include <stdlib.h>
#include <string.h>
#include "task.h"

int main(int argc, char const *argv[]){
	char* infix;
	char* postfix;
	do{
		infix = get_str();
		if(infix){
			postfix = (char*)malloc(strlen(infix));
			int len = infix_to_postfix(infix, postfix);
			show(postfix, len);
			clean(infix, postfix);
		}
	}while(infix);
	return 0;
}


































/*
	char* s = NULL;
	do{
		s = get_str();
		if(s){
			free(s);
		}
	}while(s);*/