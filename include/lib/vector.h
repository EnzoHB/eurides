#include <stddef.h>

#ifndef VECTOR_H
#define VECTOR_H

// Generic dinamic array which grows on insertion
// and shrinks on deletion. 
typedef struct Vector {
    size_t length;
    size_t capacity;
    size_t element_size;
    void* elements;
} Vector;

// Initializes the vector. By default, the vector has a 
// capacity of element_size which doubles if exceeded
Vector *vector_init(Vector* vector, size_t element_size);

// Pushes the pointer to the end of the array
size_t vector_push(Vector* vector, void* p);

// Gets the path to a file and populates the vector based on the file contents
Vector *vector_populate_from_file(Vector* vector, char *path);

#endif