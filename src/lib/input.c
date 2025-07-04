#include "lib/vector.h"
#include "keycodes.h"

typedef struct Input
{
    int cursor_position;
    Vector vector;
} Input;


int input_init(Input *input)
{
    input->cursor_position = 0;
    vector_init(&input->vector, sizeof(char));
    char terminator = '\0';
    vector_push(&input->vector, &terminator);
}

void *input_value(Input *input)
{
    return input->vector.elements;
}

int input_insert_at_cursor(Input *input, char ch)
{
    vector_insert(&input->vector, input->cursor_position, &ch);
}

int input_remove_at_cursor(Input *input)
{
    vector_remove(&input->vector, input->cursor_position);
}

int input_set_cursor(Input *input, int index)
{
    if (index < 0 || index > input->vector.length - 1)
    {
        return 1;
    }

    input->cursor_position = index;
    return 0;
}

int input_value_length(Input *input)
{
    return input->vector.length - 1;
};

int input_text_controller_v2(Input *input, int ch)
{
    if (
        '0' <= ch && ch <= '9' ||
        'a' <= ch && ch <= 'z' || 
        'A' <= ch && ch <= 'Z' ||
        ch == ' ' || 
        ch == '-')
    {
        input_insert_at_cursor(input, ch);
        input_set_cursor(input, input->cursor_position + 1);
        return 0;
    };

    switch (ch)
    {
        case DELETE: 
        {
            input_remove_at_cursor(input);
            input_set_cursor(input, input->cursor_position - 1);
            return 0;
        }
        case ARROW_LEFT:
        {
            return input_set_cursor(input, input->cursor_position - 1);
        }
        case ARROW_RIGHT:
        {
            
            return input_set_cursor(input, input->cursor_position + 1);
        }
    }

    return 3;
}