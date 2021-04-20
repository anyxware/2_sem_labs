#include <stdio.h>
#include "list.h"
#ifndef C_TABLE_H
#define C_TABLE_H
#define N 30
#define M 20

typedef struct{
	char operation[M];
	LIST* arguments;
	int cap;
}Bufer;

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
	int last_release;
	long int pos;
}KeySpace1;

typedef struct {
	int busy;
	int dead;
	int key;
	long int pos;
}KeySpace2;

typedef struct {
	int csize;
	int msize;
}Table;

void iterator_print_table(Table* table, FILE* file);

void create_table(FILE* file, Table* table, int msize);
int insert_table(FILE* file, Table* table, const char key1[N], int key2, InfoS infos);
InfoS* KS1_1_search_table(FILE* file, Table* table, char key1[N], int key2);
void KS1_1_delete_table(FILE* file, Table* table, char key1[N], int key2);
InfoS* KS2_1_search_table(FILE* file, Table* table, char* key1, int key2);
void KS2_1_delete_table(FILE* file, Table* table, char* key1, int key2);
InfoSR* KS1_2_search_table(FILE* file, Table* table, char key1[N]);
void KS1_2_delete_table(FILE* file, Table* table, char key1[N]);
InfoS* KS2_2_search_table(FILE* file, Table* table, int key2);
void KS2_2_delete_table(FILE* file, Table* table, int key2);
InfoS* search_releases_table(FILE* file, Table* table, char key1[N], int rel);
void delete_releases_table(FILE* file, Table* table, char key1[N], int rel);
void show_table(FILE* file, Table* table);
void clear_table();

#endif