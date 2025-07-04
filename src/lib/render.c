#include <stdio.h>
#include <unistd.h>
#include "lib/render.h"
#include "lib/utils.h"
#include <string.h>

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

void html_currency(int x)
{
    char y[32];
    int_to_numeral(x, y, sizeof(y));
    _html_currency((char *)&y, strlen(y));
}

void _html_currency(char *currency, int j)
{
    html_span("R$ ");

    switch (j)
    {
        case 0:
        case 1:
        case 2:

            html_span("0,");
            
            for (int k = 0; k < 2 - j; k++)
            {
                html_single_char('0');
            }

            for (int k = 0; k < j; k++)
            {
                html_single_char(*(currency + k));
            }

            break;
        
        default:    

            for (int k = 0; k < j - 2; k++)
            {
                html_single_char(*(currency + k));
            }

            html_single_char(',');
            html_single_char(*(currency + j - 2));
            html_single_char(*(currency + j - 1));

        break;
    };
};

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

void terminal_set_prefix(char *line)
{
    // Create a function that helps prefixing pages  
}

void terminal_clear_prefix()
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

