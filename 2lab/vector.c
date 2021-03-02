#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

void vector_init(VECTOR* Vector, int size) {
    Vector->data = (char*)malloc(size*(sizeof(char)));
    Vector->cap = 0;
    Vector->size = size;
}

int vector_append(VECTOR* Vector, char x){
	if(Vector->cap + 1 == Vector->size) return 0;
	Vector->data[Vector->cap++] = x;
	return 1;
} 

int vector_insert(VECTOR* Vector, char x, int i){
	if(Vector->cap + 1 == Vector->size) return 0;
	char tmp1 = x;
	char tmp2;
	Vector->cap++;
	for(int j = i; j < Vector->cap; j++){
		tmp2 = Vector->data[j];
		Vector->data[j] = tmp1;
		tmp1 = tmp2;
	}
	return 1;
}

char vector_pop(VECTOR* Vector, int i){
	if(i > Vector->cap) return 0;
	if(!Vector->cap) return 0;
	Vector->cap--;
	if(i == -1) i = Vector->cap + 1;
	char tmp = Vector->data[i-1];
	for(int j = i; j < Vector->cap - i + 1; j++){
		Vector->data[j] = Vector->data[j+1];
	}
	return tmp;
}

void vector_clear(VECTOR* Vector){
	free(Vector->data);
}

void vector_print(VECTOR* Vector){
	for(int i = 0; i < Vector->cap; i++){
		printf("%c", Vector->data[i]);
	}
	printf("\n");
}

/*
int main(int argc, char const *argv[])
{
	VECTOR Vector;
	vector_init(&Vector, 10);
	vector_append(&Vector, 'q');
	vector_print(&Vector);
	vector_append(&Vector, 'w');
	vector_print(&Vector);
	char c = vector_pop(&Vector, -1);
	//vector_print(&Vector);
	printf("%c\n", c);
	c = vector_pop(&Vector, -1);
	//vector_print(&Vector);
	printf("%c\n", c);
	return 0;
}*/
