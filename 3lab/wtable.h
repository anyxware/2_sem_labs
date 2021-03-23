#ifndef TABLE_H
#define TABLE_H
#define N 30

typedef struct {
	int x;
	int y;
	int len;
}Info;

typedef struct {
	Info info;
	char* string;
}InfoS;

typedef struct {
	Info info;
	char* string;
	int release;
}InfoSR;

typedef struct {
	Info info;
	int ind1;
	int ind2;
	int del;
}Item;

typedef struct {
	char key[N];
	int release;
	long int pos;
}KeySpace1;

typedef struct {
	char c;
	int busy;
	int key;
	long int pos;
}KeySpace2;

typedef struct {
	int csize;
	int msize;
}Table;

#endif