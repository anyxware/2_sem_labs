#ifndef VECT_H
#define VECT_H

typedef struct{
	void* data;
	int size;
	int cap;
	size_t element_size;
}VECTOR;

void vector_init(VECTOR* Vector, int size, int element_size);
int vector_append(VECTOR* Vector, void* x);
void* vector_pop(VECTOR* Vector, int i);
int vector_insert(VECTOR* Vector, void* x, int i);
void vector_remove(VECTOR* Vector, void* x);
void vector_clear(VECTOR* Vector);
void vector_print(VECTOR* Vector);

#endif //VECT_H