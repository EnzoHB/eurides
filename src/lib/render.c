#include <stdio.h>
#include "lib/render.h"

static int height = 0;

// https://www.reddit.com/r/learnprogramming/comments/o7xae7/best_way_to_clear_terminal_in_c/
void clear_screen() 
{
    printf("\e[1;1H\e[2J");
    height = 0;
}

// Breaks a line in the console
void br()
{
    printf("\n");
    height++;
}

int get_current_height()
{
    return height;
};

// Controls wheter text rendered is bold or not 
// depending on a flag
void strong(int open)
{
    if (open)
    {
        printf("\033[1m");
    }

    if (open == 0)
    {
        printf("\033[0m");
    }
}

// Prints a line of text, alias to printf
void span(char line[])
{
    printf("%s", line);
}

// Renders a paragraph of text
void p(char line[])
{
    span(line);
    br();
}

// Renders a null terminating array of strings to the console
void render(char* lines[])
{   
    int counter = 0;

    for (int i = 0; lines[i] != NULL; i++)
    {
        p(lines[i]);
    }
}