#include "lib/vector.h"
#include <stdlib.h>
// Access to memcpy and memmov
#include <string.h>
#include <stdio.h>

static int resize_to_fit(Vector *vector, int length)
{
    if (vector->capacity < length)
    {
        while(1)
        {
            vector->capacity *= 2;

            if (vector->capacity >= length)
            {
                break;
            };
        }
    }
    else if (vector->capacity >= length) 
    {
        if (vector->capacity == 1 && length < 1)
        {
            return 1;
        }

        while(1)
        {
            int half = vector->capacity / 2;

            if (length <= half)
            {
                vector->capacity = half;;
                break;
            };

            return 0;
        }
    }

    void *p = realloc(vector->elements, vector->element_size * vector->capacity);
                
    if (p == NULL)
    {
        return 1;
    }

    vector->elements = p;
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

int vector_push(Vector *vector, void *p)
{
    return vector_insert(vector, vector->length, p);
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

int vector_insert(Vector *vector, int index, void *p)
{
    if (index < 0 || index > vector->length)
    {
        return 1; 
    }

    if (resize_to_fit(vector, vector->length + 1))
    {
        return 1;
    };

    memmove(
        (char *)vector->elements + vector->element_size * (index + 1), 
        (char *)vector->elements + vector->element_size * (index), 
        (vector->length - index) * vector->element_size
    );
    memcpy(
        (char *)vector->elements + vector->element_size * (index), 
        p, 
        vector->element_size
    );
    return ++vector->length;
}

int vector_remove(Vector *vector, int index)
{
    if (index < 0 || index >= vector->length)
    {
        return 1; 
    };

    memmove((char *)vector->elements + index, vector->elements + index + 1, (vector->length - (index + 1)) * vector->element_size);
    resize_to_fit(vector, vector->length - 1);
    return --vector->length;
}