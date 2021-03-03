#ifndef LIST_H
#define LIST_H

typedef struct ITEM{
    //char data;
    void* data;
    struct ITEM *next;
}ITEM;

typedef struct{
    ITEM *head;
    ITEM *tail;
} LIST;

void list_init(LIST* List);
int list_append(LIST* List, void* data, size_t data_size);
void* list_pop(LIST* List, int i);
int list_insert(LIST *List, void* data, int i, size_t data_size);
void list_remove(LIST* List, void* data, size_t data_size);
void list_clear(LIST* list);
void list_extend(LIST* List, LIST* List1);
void list_print(LIST* List);

#endif //LIST_H



//int list_append(LIST* List, char x);
//void list_print(LIST* List);
//void list_remove(LIST *List, void* x);
//char list_pop(LIST* List, int i);
//int list_insert(LIST *List, char x, int i);
