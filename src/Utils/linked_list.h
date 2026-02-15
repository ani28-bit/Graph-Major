#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "common.h"

typedef struct ListNode {

    void* data;
    struct ListNode* next;
} ListNode;

typedef struct LinkedList {

    ListNode* head;
    ListNode* tail;
    int size;
} LinkedList;

LinkedList* list_create();
void list_append(LinkedList* list, void* data);

void* list_get(LinkedList* list, int index);
void* list_remove_first(LinkedList* list); 

void list_clear(LinkedList* list);
void list_free(LinkedList* list);
void list_foreach(LinkedList* list, void (*callback)(void* data));

#endif