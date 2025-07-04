#include <stddef.h>
#ifndef TABLE_H
#define TABLE_H
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1

typedef struct Cell {
    int bold;
    int align; 
    size_t length;
    // Internal buffer in order to prersist, without dinamica memory allocation
    // the values the are going to be pretty printed
    char text[128];
} Cell;

void inline_component_table(Cell *cells, int width, int height);

#endif