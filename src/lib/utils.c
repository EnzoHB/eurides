#include <string.h>
#include <stdio.h>
#include "keycodes.h"

void int_to_numeral(int x, char* buffer, size_t buffer_size)
{
    if (buffer_size == 0)
    {
        return;
    };
    // The output come backwards to first we need a pointer that points to
    // the end of the array
    char* p = buffer + buffer_size - 1;
    // Deference, insert null terminator, decrement;
    *(p--) = '\0';
    int negative = 0;


    if (x < 0)
    {
        negative++;
        // Internal representation of integers allows us
        // to do this simple bitwise trick
        x = ~x + 1;
    };

    if (x == 0)
    {
        *(p--) = '0';
    }
    else
    {
        // While our quotient is not zero
        // after many divisions, we are going to keep 
        // moduloing x. Also, we add a guard clause in
        // case we are about go out of the buffer
        while (x != 0 && p > buffer)
        {
            // Here, we use '0' to get to the starting point of our ASCII
            // conversion table. As every digit is ordered in sequence, we 
            // can just add our remainder to the zero decimal equivalent
            *(p--) = '0' + x % 10;
            x /= 10;
        }
    
    };

    if (negative && p > buffer)
    {
        *(p--) = '-';
    }

    // We have filled the buffer
    if (p == buffer)
    {
        return;
    }

    // Gets the current position of the pointer, which is guaranteed
    // to be greater than buffer, that is, the beginning of it
    // Subtracts it from the size of the buffer;
    // Copies p + generated chars to the beginning of 
    // the buffer, accounting for memory overlap.
    memmove(buffer, p + 1, (buffer + buffer_size) - (p + 1));
    // Reasoning:  // [B][][P][1][3][5][\0] 
    // Derived buffer_size - (p + 1)
    // Interesting enough, (buffer + buffer_size) - (p + 1) works and 
    // buffer_size - (p + 1) does not. This is because the subtraction operation
    // actually occurs in the first element, that is, is not commutative, which in turn
    // means we are trying to subtract a pointer from size_t. The opposite is true though:
    // we indeed can subtract a size_t from a pointer and also, as the addition is commutative.
    // the order by which we sum size_t and pointer does not matter
}

int input_text_controller(int ch, char *p_field, int *p_field_length, int *p_cursor_pos, int MAX_LENGTH)
{
    if (*p_field_length >= MAX_LENGTH)
    {
        return 1;
    }

    if (
        '0' <= ch && ch <= '9' ||
        'a' <= ch && ch <= 'z' || 
        'A' <= ch && ch <= 'Z' ||
        ch == ' ' || 
        ch == '-')
    {
        // Shifts position of the characters to the right
        memmove(p_field + *p_cursor_pos + 1, p_field + *p_cursor_pos, *p_field_length - *p_cursor_pos);
        *(p_field + (*p_cursor_pos)++) = ch;
        (*p_field_length)++;
        return 0;
    };

    switch (ch)
    {
        case DELETE: 
        {
            if (*p_field_length < 1 && *p_cursor_pos < 1)
            {
                return 2;
            }

            // Shifts the position of the characters to the left
            memmove(p_field + *p_cursor_pos - 1, p_field + *p_cursor_pos, *p_field_length - *p_cursor_pos);
            (*p_cursor_pos)--;
            (*p_field_length)--;
        
            return 0;
        }
        case ARROW_LEFT:
        {
            if (*p_cursor_pos > 0)
            {
                (*p_cursor_pos)--;
            }

            return 0;
        }
        case ARROW_RIGHT:
        {
            if (*p_cursor_pos < *p_field_length)
            {
                (*p_cursor_pos)++;
            }

            return 0;
        }
    }

    return 3;
}

void format_price(const char *input, char *output) 
{
    size_t len = strlen(input);
    // Prefix: "R$ "
    output[0] = 'R';
    output[1] = '$';
    output[2] = ' ';;

    if (len >= 3) {
        // Integer segment
        size_t integer_len = len - 2;
        memcpy(output + 3, input, integer_len);
        output[3 + integer_len] = ',';
        // Decimal segment
        output[3 + integer_len + 1] = input[len - 2];
        output[3 + integer_len + 2] = input[len - 1];
        // Null terminator
        output[3 + integer_len + 3] = '\0';
    } 
    else {
        // Integer segment is zero
        output[3] = '0';
        output[4] = ',';

        if (len == 2) 
        {
            output[5] = input[0];
            output[6] = input[1];
        } 
        else if (len == 1) 
        {
            output[5] = '0';
            output[6] = input[0];
        } 
        else 
        {
            output[5] = '0';
            output[6] = '0';
        }

        output[7] = '\0';
    }
}
