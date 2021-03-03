//cc -o lab2  main.c stack.c -D VECT_IMP -D STACK_TYPE=STACK_VECT
//cc -o lab2  main.c stack.c -D LIST_IMP -D STACK_TYPE=STACK_LIST
//#define LIST_IMP
//#define STACK_TYPE STACK_LIST
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "list.h"
#include "vector.h"

struct CONTEINER{
	#ifdef STACK_LIST
		LIST tank;
	#else 
		VECTOR tank;
	#endif
};

void stack_init(STACK* Stack, int size, int element_size){
	Stack->conteiner = (CONTEINER*)calloc(1, sizeof(CONTEINER));
	#ifdef STACK_LIST
		list_init(&Stack->conteiner->tank);
	#else
		vector_init(&Stack->conteiner->tank, size, element_size);
	#endif
}

int stack_push(STACK* Stack, void* operand){
	int status;
	#ifdef STACK_LIST
		status = list_append(&Stack->conteiner->tank, operand, sizeof(operand));
	#else
		status = vector_append(&Stack->conteiner->tank, operand);
	#endif
	return status;
}

void* stack_pop(STACK* Stack){
	void* status;
	#ifdef STACK_LIST
		status = list_pop(&Stack->conteiner->tank, -1);
	#else
		status = vector_pop(&Stack->conteiner->tank, -1);
	#endif
	return status;
}

void stack_clear(STACK* Stack){
	#ifdef STACK_LIST
		list_clear(&Stack->conteiner->tank);
	#else
		vector_clear(&Stack->conteiner->tank);
	#endif
	free(Stack->conteiner);
}

/*

int main(int argc, char const *argv[])
{
	STACK Stack;
	stack_init(&Stack,0,0);
	char c = '(';
	stack_push(&Stack, (void*)&c);
	printf("%c", *((char*)stack_pop(&Stack)));
	stack_push(&Stack, (void*)&c);
	printf("%c", *((char*)stack_pop(&Stack)));
	return 0;
}

*/
















/*
#ifdef VECT_IMP//-----------------------------------------

void init_stack(STACK_VECT* stack){
	stack->top = 0;
	printf("vector is using r n\n");
}

int write_stack(STACK_VECT* stack, char operand){
	if(stack->top == 80){
		printf("stackoverflow\n");
		return 0;
	}
	stack->data[stack->top++] = operand;
	return 1;
}

char read_stack(STACK_VECT* stack){
	if(stack->top == 0) return 0;
	return stack->data[--stack->top];
}

#endif //VECT_IMP------------------------------------------

#ifdef LIST_IMP //-----------------------------------------

void init_stack(STACK_LIST* stack){
	stack->top = NULL;
	printf("list is using r n\n");
}	

int write_stack(STACK_LIST* stack, char operand){
	ITEM* tmp = (ITEM*)malloc(sizeof(ITEM));
	if(!tmp) return 0;
	tmp->data = operand;
	tmp->next = stack->top;
	stack->top = tmp;
	return 1;
}

char read_stack(STACK_LIST* stack){
	if(!(stack->top)) return 0;
	char c;
	ITEM* tmp = stack->top;
	stack->top = tmp->next;
	tmp->next = NULL;
	c = tmp->data;
	free(tmp);
	return c;
}

#endif //LIST_IMP------------------------------------------

*/