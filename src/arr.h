#ifndef ARR_H
#define ARR_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void** data;
    size_t size;
    size_t capacity;
} arr_t;

arr_t* arr_create(size_t initial_capacity);
void arr_destroy(arr_t* arr);
bool arr_push(arr_t* arr, void* element);
void* arr_get(arr_t* arr, size_t index);
bool arr_set(arr_t* arr, size_t index, void* element);
bool arr_remove(arr_t* arr, size_t index);
size_t arr_size(arr_t* arr);
bool arr_resize(arr_t* arr, size_t new_capacity);

#endif
