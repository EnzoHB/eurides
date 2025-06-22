#include "lib/io.h"
#include <stdio.h>
#include <stdlib.h>

static char product_path[] = "../../../data/products.dat";

void* alloc_structs_from_file(char path[], size_t size_of_struct, size_t* length)
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
    *length = file_size / size_of_struct;
    rewind(file);
    void* memory = malloc(file_size);
    // Memory could not be allocated 
    if (memory == NULL)
    {
        fclose(file);
        return NULL;
    }

    size_t bytes = fread(memory, file_size, 1, file);

    // we did not read the corret number of bytes
    if (bytes != file_size)
    {
        free(memory);
        fclose(file);
        return NULL;
    }

    fclose(file);

    return memory;
};

