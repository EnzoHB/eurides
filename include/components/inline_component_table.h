#include <stddef.h>
#ifndef TABLE_H
#define TABLE_H
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1

typedef struct Cell {
    int bold;
    int align; 
    size_t length;
    char *text;
} Cell;

void inline_component_table(Cell **cells, int width, int height);

#endif