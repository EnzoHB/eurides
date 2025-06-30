#include <string.h>
#include <stdio.h>

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