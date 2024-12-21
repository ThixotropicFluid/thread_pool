#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

// Helper function to create a new node
static LinkedListNode* linked_list_create_node(void* data) {
    LinkedListNode* node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    if (node) {
        node->data = data;
        node->next = NULL;
        node->prev = NULL;
    }
    return node;
}

// Create a new linked list
LinkedList* linked_list_create() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
    return list;
}

// Destroy a linked list
void linked_list_destroy(LinkedList* list, void (*destructor)(void*)) {
    if (!list) return;

    LinkedListNode* current = list->head;
    while (current) {
        LinkedListNode* next = current->next;
        if (destructor) {
            destructor(current->data);
        }
        free(current);
        current = next;
    }
    free(list);
}

// Add a new element to the front of the list
void linked_list_push_front(LinkedList* list, void* data) {
    if (!list) return;

    LinkedListNode* node = linked_list_create_node(data);
    if (!node) return;

    node->next = list->head;
    if (list->head) {
        list->head->prev = node;
    }
    list->head = node;

    if (!list->tail) {
        list->tail = node; // If list was empty
    }
    list->size++;
}

// Add a new element to the back of the list
void linked_list_push_back(LinkedList* list, void* data) {
    if (!list) return;

    LinkedListNode* node = linked_list_create_node(data);
    if (!node) return;

    node->prev = list->tail;
    if (list->tail) {
        list->tail->next = node;
    }
    list->tail = node;

    if (!list->head) {
        list->head = node; // If list was empty
    }
    list->size++;
}

// Remove and return the front element of the list
void* linked_list_pop_front(LinkedList* list) {
    if (!list || !list->head) return NULL;

    LinkedListNode* node = list->head;
    void* data = node->data;

    list->head = node->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL; // If list becomes empty
    }

    free(node);
    list->size--;
    return data;
}

// Remove and return the back element of the list
void* linked_list_pop_back(LinkedList* list) {
    if (!list || !list->tail) return NULL;

    LinkedListNode* node = list->tail;
    void* data = node->data;

    list->tail = node->prev;
    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL; // If list becomes empty
    }

    free(node);
    list->size--;
    return data;
}

// Return the front element without removing it
void* linked_list_peek_front(const LinkedList* list) {
    if (!list || !list->head) return NULL;
    return list->head->data;
}

// Return the back element without removing it
void* linked_list_peek_back(const LinkedList* list) {
    if (!list || !list->tail) return NULL;
    return list->tail->data;
}

// Append the contents of one list to another without modifying the source
void linked_list_append(LinkedList* destination, const LinkedList* source) {
    if (!destination || !source) return;

    LinkedListNode* current = source->head;
    while (current) {
        // Copy the data pointer to the new list
        linked_list_push_back(destination, current->data);
        current = current->next;
    }
}
