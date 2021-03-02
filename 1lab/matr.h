#ifndef LAB1_H
#define LAB1_H

typedef struct{
	int n;
	int* string;
}line;

typedef struct{
	int m;
	line* matrix;
}matr;

matr read_matr();
matr new_matr(matr M);
void show_matr(matr M, char* s);
void delete_matr(matr M);
#endif

