#include <stddef.h>

#ifndef VECTOR_H
#define VECTOR_H

// Generic dinamic array which grows on insertion
// and shrinks on deletion. 
typedef struct Vector {
    int length;
    int capacity;
    size_t element_size;
    void* elements;
} Vector;

// Initializes the vector. By default, the vector has a 
// capacity of element_size which doubles if exceeded
Vector *vector_init(Vector* vector, size_t element_size);

// Pushes the pointer to the end of the array
int vector_push(Vector* vector, void* p);

// Gets the path to a file and populates the vector based on the file contents
Vector *vector_populate_from_file(Vector* vector, char *path);

int vector_insert(Vector *vector, int index, void *p);
int vector_remove(Vector *vector, int index);

#endif