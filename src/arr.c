#include "arr.h"
#include <string.h>

arr_t* arr_create(size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 4;
    }

    arr_t* arr = malloc(sizeof(arr_t));
    if (!arr) {
        return NULL;
    }

    arr->data = malloc(initial_capacity * sizeof(void*));
    if (!arr->data) {
        free(arr);
        return NULL;
    }

    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void arr_destroy(arr_t* arr) {
    if (!arr) return;
    
    if (arr->data) {
        free(arr->data);
    }
    free(arr);
}

bool arr_push(arr_t* arr, void* element) {
    if (!arr) return false;

    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (!arr_resize(arr, new_capacity)) {
            return false;
        }
    }

    arr->data[arr->size++] = element;
    return true;
}

void* arr_get(arr_t* arr, size_t index) {
    if (!arr || index >= arr->size) {
        return NULL;
    }
    return arr->data[index];
}

bool arr_set(arr_t* arr, size_t index, void* element) {
    if (!arr || index >= arr->size) {
        return false;
    }
    arr->data[index] = element;
    return true;
}

bool arr_remove(arr_t* arr, size_t index) {
    if (!arr || index >= arr->size) {
        return false;
    }

    for (size_t i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }

    arr->size--;
    return true;
}

size_t arr_size(arr_t* arr) {
    return arr ? arr->size : 0;
}

bool arr_resize(arr_t* arr, size_t new_capacity) {
    if (!arr || new_capacity < arr->size) {
        return false;
    }

    void** new_data = realloc(arr->data, new_capacity * sizeof(void*));
    if (!new_data) {
        return false;
    }

    arr->data = new_data;
    arr->capacity = new_capacity;
    return true;
}
