#include <stdio.h>
#include <unistd.h>
#include "lib/render.h"

static int page_height = 0;
static int page_current_y = 0;
static int window_start_y = 0;
static int window_end_y = 40;
static int bold = 0;

// https://www.reddit.com/r/learnprogramming/comments/o7xae7/best_way_to_clear_terminal_in_c/
void clear_screen() 
{
    printf("\e[1;1H\e[2J");
    page_height = 0;
}

// Breaks a line in the console
void html_br()
{
    printf("\n");
    page_height++;
}

int get_current_height()
{
    return page_height;
};

int move_cursor(int row, int column) 
{
    if (row < 1 || column < 1)
    {
        return 1;
    }

    printf("\033[%d;%dH", row, column);
    return 0;
};

// Controls wheter text rendered is bold or not 
// depending on a flag
void html_strong(int open)
{
    if (open)
    {
        bold = 1;
        printf("\033[1m");
    }

    if (open == 0)
    {
        bold = 0;
        printf("\033[0m");
    }
}

// Prints a line of text, alias to printf
void html_span(char *line)
{
    printf("%s", line);
}

void html_single_char(char letter)
{
    printf("%c", letter);
}

// Renders a paragraph of text
void html_p(char *line)
{
    html_span(line);
    html_br();
}

void term_set_prefix(char *line)
{
    // Create a function that helps prefixing pages  
}

void term_clear_prefix()
{

}


// Renders a null terminating array of strings to the console
void render(char **lines)
{   
    int counter = 0;

    for (int i = 0; lines[i] != NULL; i++)
    {
        html_p(lines[i]);
    }
}

