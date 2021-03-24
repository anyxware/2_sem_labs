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

void create_wtable(int msize);

int insert_wtable(char key1[N], int key2, InfoS infos);

InfoS* KS1_1_search_wtable(char key1[N], int key2);

void KS1_1_delete_wtable(char key1[N], int key2);

InfoS* KS2_1_search_wtable(char* key1, int key2);

void KS2_1_delete_wtable(char* key1, int key2);

InfoSR* KS1_2_search_wtable(char key1[N]);

void KS1_2_delete_wtable(char key1[N]);

InfoS* KS2_2_search_wtable(int key2);

void KS2_2_delete_wtable(int key2);

InfoS* search_releases_wtable(char key1[N], int rel);

void delete_releases_wtable(char key1[N], int rel);

void show_wtable();

void clear_wtable();

void garbage_collector_wtable();


#endif