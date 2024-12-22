#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h> // for size_t

typedef struct LinkedListNode {
    void* data;                     // Pointer to the data
    struct LinkedListNode* next;    // Pointer to the next node
    struct LinkedListNode* prev;    // Pointer to the previous node
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode* head;           // Pointer to the head of the list
    LinkedListNode* tail;           // Pointer to the tail of the list
    size_t size;                    // Number of elements in the list
} LinkedList;

// Function prototypes

/**
 * @brief Creates a new linked list.
 * 
 * @return LinkedList* Pointer to the created linked list.
 */
LinkedList* linked_list_constructor();

/**
 * @brief Destroys a linked list and frees memory.
 * 
 * @param list Pointer to the linked list.
 * @param destructor Function to free the data in each node.
 */
void linked_list_destructor(LinkedList* list, void (*destructor)(void*));

/**
 * @brief Adds a new element to the front of the list.
 * 
 * @param list Pointer to the linked list.
 * @param data Pointer to the data to add.
 */
void linked_list_push_front(LinkedList* list, void* data);

/**
 * @brief Adds a new element to the back of the list.
 * 
 * @param list Pointer to the linked list.
 * @param data Pointer to the data to add.
 */
void linked_list_push_back(LinkedList* list, void* data);

/**
 * @brief Removes and returns the front element of the list.
 * 
 * @param list Pointer to the linked list.
 * @return void* Pointer to the removed data, or NULL if the list is empty.
 */
void* linked_list_pop_front(LinkedList* list);

/**
 * @brief Removes and returns the back element of the list.
 * 
 * @param list Pointer to the linked list.
 * @return void* Pointer to the removed data, or NULL if the list is empty.
 */
void* linked_list_pop_back(LinkedList* list);

/**
 * @brief Returns the front element without removing it.
 * 
 * @param list Pointer to the linked list.
 * @return void* Pointer to the front data, or NULL if the list is empty.
 */
void* linked_list_peek_front(const LinkedList* list);

/**
 * @brief Returns the back element without removing it.
 * 
 * @param list Pointer to the linked list.
 * @return void* Pointer to the back data, or NULL if the list is empty.
 */
void* linked_list_peek_back(const LinkedList* list);

/**
 * @brief Appends the contents of one list to another.
 * 
 * @param destination Pointer to the destination list.
 * @param source Pointer to the source list (not mutated).
 */
void linked_list_append(LinkedList* destination, const LinkedList* source);

/**
 * @brief prints the list
 * 
 * @param list list
 * @param format_data Pointer to formating function 
 */
void linked_list_print(const LinkedList* list, char* (*format_data)(void* data));
#endif // LINKED_LIST_H