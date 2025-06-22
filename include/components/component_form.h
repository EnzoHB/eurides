#ifndef FORM_H
#define FORM_H

typedef struct Field {
    char label[128];
    void* field;
} Field;

void component_form(void (*header)(), Field* fields, void (*footer)());

#endif