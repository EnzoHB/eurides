#include "lib/go_to.h"

void go_to(int (*page)()) 
{
    while(1)
    {
        int back = page();
        
        if (back != 0) 
        {
            break;
        }
    };
}