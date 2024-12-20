#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

typedef struct Node {
    void* data;

    struct Node* next;
    struct Node* prev;
} Node;
typedef struct LinkedList {
    struct Node* head;
    struct Node* tail;

    int length;

    void (*insert_head)(struct LinkedList* list, void* data, unsigned int size);
    void (*insert_tail)(struct LinkedList* list, void* data, unsigned int size);
    void (*insert)(struct LinkedList* list, int position, void* data, unsigned int size);
    void* (*peek_tail)(struct LinkedList* list);
    void* (*peek_head)(struct LinkedList* list);
    void* (*pop_tail)(struct LinkedList* list);
    void* (*pop_head)(struct LinkedList* list);
} LinkedList;


extern struct LinkedList linked_list_constructor();
extern void linked_list_destructor();
extern void linked_list_insert_head(struct LinkedList* list, void* data, unsigned int size);
extern void linked_list_insert_tail(struct LinkedList* list, void* data, unsigned int size);
extern void linked_list_insert(struct LinkedList* list, int position, void* data, unsigned int size);
extern void* linked_list_peek_tail(struct LinkedList* list);
extern void* linked_list_peek_head(struct LinkedList* list);
extern void* linked_list_pop_tail(struct LinkedList* list);
extern void* linked_list_pop_head(struct LinkedList* list);

#endif