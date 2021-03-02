#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void list_init(LIST* List){
    //List = (LIST*)calloc(1, sizeof(LIST));
    List->head = NULL;
    List->tail = NULL;
}

int list_append(LIST* List, char x){
    ITEM* ptr = (ITEM*)malloc(sizeof(ITEM));
    if(!ptr) return 0;
    ptr->data = x;
    ptr->next = NULL;
    if(!List->head){
        List->head = ptr;
        List->tail = ptr;
    }else{
        List->tail->next = ptr;
        List->tail = ptr;
    }
    return 1;
}

void list_extend(LIST* List, LIST* List1){
    if(!List->head){
        List->head = List1->head;
        List->tail = List1->tail;
    }else{
        List->tail->next = List1->head;
        List->tail = List1->tail;
    }
}

void list_remove(LIST* List, char x) {
    ITEM* ptr = List->head, *ptr_prev = NULL;
    while (ptr && ptr->data != x) {
        ptr_prev = ptr;
        ptr = ptr->next;
    }
    if (!ptr) {
        return;
    }
    if (ptr == List->head) {
        List->head = ptr->next;
    }
    if (ptr == List->tail) {
        List->tail = ptr_prev;
    }
    if (ptr_prev) {
        ptr_prev->next = ptr->next;
    }
    free(ptr);
}

char list_pop(LIST* List, int i){
    ITEM* ptr = List->head, *ptr_prev = NULL;
    int j = 0;
    char c;
    if(i == -1){
        while (ptr && ptr->next) {
            ptr_prev = ptr;
            ptr = ptr->next;
        }
    }else{
        while (ptr && j++ < i) {
            ptr_prev = ptr;
            ptr = ptr->next;
        }
    }
    if (!ptr) {
        return 0;
    }
    if (ptr == List->head) {
        List->head = ptr->next;
    }
    if (ptr == List->tail) {
        List->tail = ptr_prev;
    }
    if (ptr_prev) {
        ptr_prev->next = ptr->next;
    }
    c =  ptr->data;
    free(ptr);
    return c;
}

int list_insert(LIST *List, char x, int i) {
    ITEM* ptr = List->head, *ptr_prev = NULL;
    int j = 0;
    while (ptr && j++ < i) {
        ptr_prev = ptr;
        ptr = ptr->next;
    }
    ITEM *new = (ITEM *) malloc(sizeof(ITEM));
    if (!new) {
        return -1;
    }
    new->data = x;
    new->next = ptr;
    if (ptr_prev) {
        ptr_prev->next = new;
    } else {
        List->head = new;
    }
    if (!ptr) {
        List->tail = new;
    }
    return 0;
}

void list_print(LIST* List){
    ITEM* ptr = List->head;
    while(ptr){
        printf("%c", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void list_clear(LIST* List) {
    ITEM* ptr = List->head, *ptr_prev;
    while (ptr) {
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
    //free(List);
}

/*
int main(int argc, char const *argv[])
{
    LIST List, List1;
    list_init(&List);
    list_init(&List1);
    char s[] = "qwertyuiopasdfghjklzxcvbnm";
    for(int i = 0; i < 26; i++){
        list_append(&List, s[i]);
        list_append(&List1, s[i]);
    }
    list_extend(&List, &List1);
    list_print(&List);
    list_remove(&List, 'r');
    printf("\n");
    list_print(&List);
    printf("\n");
    char c = list_pop(&List, -1);
    printf("%c\n", c);
    list_print(&List);
    printf("\n");
    list_insert(&List, '7', 1);
    list_print(&List);
    return 0;
}
*/


/*
List *list_new() {
    return (List *) calloc(1, sizeof(List));
}

void list_delete(List *list) {
    Item *ptr = list->head, *ptr_prev;
    while (ptr) {
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
    free(list);
}

void list_print(const List *list) {
    Item *ptr = list->head;
    while (ptr) {
        printf("%d ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

int list_push_back(List *list, char data) {
    Item *ptr = (Item *) malloc(sizeof(Item));
    if (!ptr) {
        return -1;
    }
    ptr->data = data;
    ptr->next = NULL;
    if (!list->head) {
        list->head = ptr;
        list->tail = ptr;
    } else {
        list->tail->next = ptr;
        list->tail = ptr;
    }
    return 0;
}

int list_insert(List *list, char data) {
    Item *ptr = list->head, *ptr_prev = NULL;
    while (ptr && (ptr->data < data)) {
        ptr_prev = ptr;
        ptr = ptr->next;
    }
    Item *new = (Item *) malloc(sizeof(Item));
    if (!new) {
        return -1;
    }
    new->data = data;
    new->next = ptr;
    if (ptr_prev) {
        ptr_prev->next = new;
    } else {
        list->head = new;
    }
    if (!ptr) {
        list->tail = new;
    }
    return 0;
}

void list_remove(List *list, char data) {
    Item *ptr = list->head, *ptr_prev = NULL;
    while (ptr && ptr->data != data) {
        ptr_prev = ptr;
        ptr = ptr->next;
    }
    if (!ptr) {
        return;
    }
    if (ptr == list->head) {
        list->head = ptr->next;
    }
    if (ptr == list->tail) {
        list->tail = ptr_prev;
    }
    if (ptr_prev) {
        ptr_prev->next = ptr->next;
    }
    free(ptr);
}

char list_pope_back(List *list){

}
*/