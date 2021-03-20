#ifndef TABLE_H
#define TABLE_H

typedef struct {
	int x;
	int y;
	char *string;
}Info;

typedef struct {
	Info info;
	int release;
}InfoR;

typedef struct {
	Info info;
	int ind1;
	int ind2;
}Item;

typedef struct {
	char* key;
	int release;
	Item *item;
}KeySpace1;

typedef struct {
	int busy;
	int key;
	Item *item;
}KeySpace2;

typedef struct {
	KeySpace1 *ks1;
	KeySpace2 *ks2;
	int msize;
	int csize;
}Table;

Table* create_table(int msize);

int insert_table(Table* table,const char* key1, int key2, Info info);

//2 keys

const Info* KS1_1_search_table(Table* table, char* key1, int key2);

Info* KS1_1_copy_search_table(Table* table, char* key1, int key2);

void KS1_1_delete_table(Table* table, char* key1, int key2);

const Info* KS2_1_search_table(Table* table, char* key1, int key2);

Info* KS2_1_copy_search_table(Table* table, char* key1, int key2);

void KS2_1_delete_table(Table* table, char* key1, int key2);

//1 key

InfoR* KS1_2_search_table(Table* table, char* key1);

InfoR* KS1_2_copy_search_table(Table* table, char* key1);

void KS1_2_delete_table(Table* table, char* key1);

const Info* KS2_2_search_table(Table* table, int key2);

Info* KS2_2_copy_search_table(Table* table, int key2);

void KS2_2_delete_table(Table* table, int key2);



const Info* search_releases_table(Table* table, char* key1, int rel);

Info* copy_search_releases_table(Table* table, char* key1, int rel);

void delete_releases_table(Table* table, char* key1, int rel);

void show_table(Table* table);

void clear_table(Table* table);

#endif //TABLE_H

