#ifndef VECT_H
#define VECT_H

typedef struct{
	char* data;
	int size;
	int cap;
}VECTOR;

void vector_init(VECTOR* Vector, int size);
int vector_append(VECTOR* Vector, char x);
int vector_insert(VECTOR* Vector, char x, int i);
char vector_pop(VECTOR* Vector, int i);
void vector_clear(VECTOR* Vector);
void vector_print(VECTOR* Vector);

#endif //VECT_H