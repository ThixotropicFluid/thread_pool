
#include "data_structures/lists/linked_list.h"
#include "thread_pool/thread_pool.h"
#include "stdio.h"
#include "stdlib.h"

char* int_to_string(int* value);

int main() {

    int* data = malloc(sizeof (int) * 10);
    for(int i = 0; i < 10; i ++) {
        data[i] = i;
    }
    LinkedList* list = linked_list_constructor();
    linked_list_push_front(list, (void*)(data + 0));
    linked_list_push_front(list, (void*)(data + 1));
    linked_list_push_front(list, (void*)(data + 2));
    linked_list_push_front(list, (void*)(data + 3));
    linked_list_push_front(list, (void*)(data + 4));
    linked_list_push_front(list, (void*)(data + 5));

    linked_list_pop_front(list);

    linked_list_print(list, &int_to_string);

    linked_list_destructor(list, NULL);
    free(data);
    printf("Press anything to quit.\n");
    getchar();
}

char* int_to_string(int* value) {
    // Allocate enough space for the string representation of the integer,
    // including the null terminator.
    // Maximum size needed for an integer in decimal is 11 characters (-2147483648).
    char* str = (char*)malloc(12 * sizeof(char));
    if (!str) {
        perror("Failed to allocate memory for string");
        return NULL;
    }

    // Use snprintf to format the integer as a string.
    snprintf(str, 12, "%d", *value);

    return str;
}