#include "lib/vector.h"
#include <stdlib.h>
// Access to memcpy and memmov
#include <string.h>
#include <stdio.h>

static int before_length_increment(Vector *vector)
{
    if (vector->capacity < vector->length + 1)
    {
        // Doubles the memory allocated to this vector
        void* p = realloc(vector->elements, vector->element_size * vector->capacity * 2);

        if (p == NULL)
        {
            return 1;
        }

        vector->capacity *= 2;
        vector->elements = p;
    }

    return 0;
};

static int before_length_decrement(Vector *vector)
{
    if (vector->capacity == 1)
    {
        return 1;
    }

    if (vector->length <= vector->capacity / 2)
    {
        // Halves the memory allocated to this vector
        void* p = realloc(vector->elements, vector->element_size * vector->capacity / 2);

        if (p == NULL)
        {
            return 1;
        }

        vector->capacity /= 2;
        vector->elements = p;
    }

    return 0;
}

Vector *vector_init(Vector *vector, size_t element_size)
{
    vector->length = 0;
    vector->capacity = 1;
    vector->element_size = element_size;
    vector->elements = malloc(vector->capacity * vector->element_size);
    return vector;
}

size_t vector_push(Vector *vector, void *p)
{
    if (before_length_increment(vector))
    {
        return vector->length; 
    }
    // Here we have to cast the vector->elements to a char pointer to allow for 
    // pointer arithmetic. As every char occuppies exactly one byte, we can move
    // byte by byte when copying memory
    memcpy((char*)vector->elements + vector->length * vector->element_size, p, vector->element_size);
    return ++vector->length;
};

Vector *vector_populate_from_file(Vector *vector, char *path)
{
    FILE* file = fopen(path, "rb");

    // Could not open file
    if (file == NULL)
    {
        return NULL;
    };

    fseek(file, 0, SEEK_END);
    // ftell() returns the number of bytes from the begining of the file
    // to the position of the cursor. As we have set the cursor at the 
    // end with fseek(), ftell() will return the file size in bytes
    size_t file_size = ftell(file);
    // Here we derefence length and pass in the numbers of structs that this file contains
    // we have to pass in the address of a lenght variable so that we can later iterate
    // over the pointers
    vector->length = file_size / vector->element_size;

    // Calculate the minimum capacity
    while(vector->capacity < vector->length)
    {
        // Bitshift to the left to double
        vector->capacity <<= 1;
    }

    rewind(file);
    void* mem = realloc(vector->elements, vector->capacity);
    // Memory could not be allocated 
    if (mem == NULL)
    {
        fclose(file);
        return NULL;
    }

    size_t bytes = fread(mem, file_size, 1, file);

    // we did not read the corret number of bytes
    if (bytes != file_size)
    {
        free(mem);
        fclose(file);
        return NULL;
    }

    vector->elements = mem;
    fclose(file);
    return vector;
};