// Now it has come to me that this actually
// means If not definition, then define
#include <stddef.h>

#ifndef UTILS_H
#define UTILS_H

void int_to_numeral(int x, char* buffer, size_t buffer_size);
int input_text_controller(int ch, char *p_field, int *p_field_length, int *p_cursor_pos, int MAX_LENGTH);
void format_price(const char *input, char *output);

#endif