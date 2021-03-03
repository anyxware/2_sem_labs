//!!!
//status = vector_pop(&List, i);
//free(status);
//----
//vector_clear(&List);
//!!!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

void vector_init(VECTOR* Vector, int size, int element_size) {
    Vector->data = malloc(size*element_size);
    Vector->cap = 0;
    Vector->size = size;
    Vector->element_size = element_size;
}

int vector_append(VECTOR* Vector, void* x){
	if(Vector->cap + 1 == Vector->size) return 0;
	memcpy(Vector->data + (Vector->cap++) * Vector->element_size, x, Vector->element_size);
	return 1;
}

void* vector_pop(VECTOR* Vector, int i){
	if(i > Vector->cap) return 0;
	if(!Vector->cap) return 0;
	Vector->cap--;
	if(i == -1) i = Vector->cap + 1;
	void* tmp = malloc(Vector->element_size);
	memcpy(tmp, Vector->data + (i-1) * Vector->element_size, Vector->element_size);
	for(int j = i; j < Vector->cap - i + 1; j++){
		memcpy(Vector->data + j * Vector->element_size, Vector->data + (j + 1) * Vector->element_size, Vector->element_size);
	}
	return tmp;
}

int vector_insert(VECTOR* Vector, void* x, int i){
	if(Vector->cap + 1 == Vector->size) return 0;
	void* tmp1 = malloc(Vector->element_size);
	void* tmp2 = malloc(Vector->element_size);
	memcpy(tmp1, x, Vector->element_size);
	Vector->cap++;
	for(int j = i; j < Vector->cap; j++){
		memcpy(tmp2, Vector->data + j * Vector->element_size, Vector->element_size);
		memcpy(Vector->data + j * Vector->element_size, tmp1, Vector->element_size);
		memcpy(tmp1, tmp2, Vector->element_size);
	}
	free(tmp1);
	free(tmp2);
	return 1;
}

void vector_remove(VECTOR* Vector, void* x){
	if(!Vector->cap) return;
	Vector->cap--;
	int i = -1;
	for(int j = 0; j < Vector->cap + 1; j++){
		if(!memcmp(Vector->data + j * Vector->element_size, x, Vector->element_size))
			i = j;
	}
	if(i == -1) return;
	void* tmp = Vector->data + (i-1) * Vector->element_size;
	for(int j = i; j < Vector->cap - i + 1; j++){
		memcpy(Vector->data + j * Vector->element_size, Vector->data + (j + 1) * Vector->element_size, Vector->element_size);
	}
}

void vector_clear(VECTOR* Vector){
	free(Vector->data);
}

void vector_print(VECTOR* Vector){
	for(int i = 0; i < Vector->cap; i++){
		printf("%c", *((char*)(Vector->data + i*Vector->element_size)));
	}
	printf("\n");
}

//----------------------------------------------------------
/*
int main(int argc, char const *argv[])
{
	char c = '3';
	VECTOR Vector;
	void* status;
	vector_init(&Vector, 80, sizeof(c));
	char s[] = "qwertyuiopasdfghjklzxcvbnm";
	for( int i = 0; i < strlen(s); i++){
		vector_append(&Vector, (void*)&s[i]);
	}
	for( int i = 0; i < strlen(s); i++){
		status = vector_pop(&Vector, -1);
		if(status) printf("%c", *((char*)status));
		free(status);
	}
	for( int i = 0; i < strlen(s); i++){
		vector_append(&Vector, (void*)&s[i]);
	}

	vector_insert(&Vector, (void*)&c, 0);
	vector_print(&Vector);
	printf("\n");
	vector_remove(&Vector, (void*)&c);
	vector_print(&Vector);
	vector_clear(&Vector);
}
	/*
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


/*
void vector_print(VECTOR* Vector){
	for(int i = 0; i < Vector->cap; i++){
		printf("%c", Vector->data[i]);
	}
	printf("\n");
}*/
