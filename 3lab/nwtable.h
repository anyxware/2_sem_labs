#ifndef WTABLE_H
#define WTABLE_H
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

void iterator_print_table(Table* table, KeySpace1** ks1, FILE* file);

void create_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, int msize);
int insert_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N], int key2, InfoS infos);
InfoS* KS1_1_search_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N], int key2);
void KS1_1_delete_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N], int key2);
InfoS* KS2_1_search_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N], int key2);
void KS2_1_delete_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N], int key2);
InfoSR* KS1_2_search_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N]);
void KS1_2_delete_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N]);
InfoS* KS2_2_search_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, int key2);
void KS2_2_delete_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, int key2);
InfoS* search_releases_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N], int rel);
void delete_releases_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N], int rel);
void show_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file);
void clear_wtable(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file);
void garbage_collector_wtable();
InfoS* search_iterator_table(Table* table, KeySpace1** ks1, KeySpace2** ks2, FILE* file, char key1[N]);

#endif 