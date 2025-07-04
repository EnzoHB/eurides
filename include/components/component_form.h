#ifndef FORM_H
#define FORM_H
#include <stddef.h>

#define TEXT_FIELD 0 
#define MONEY_FIELD 1
#define NUMBER_FIELD 2

typedef struct Field {
    char label[128];
    void *field;
    int type;
} Field;

void component_form(void (*header)(), Field *fields, size_t fields_length, char *title, void (*footer)());

#endif