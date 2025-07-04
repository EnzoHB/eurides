#include "lib/vector.h"

typedef struct Input
{
    int cursor_position;
    Vector vector;
} Input;


int input_init(Input *input);
int input_value(Input *input);
int input_insert_at_cursor(Input *input, char ch);
int input_remove_at_cursor(Input *input);
int input_set_cursor(Input *input, int index);
int input_value_length(Input *input);
int input_text_controller_v2(Input *input, int ch);