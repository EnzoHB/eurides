#include "lib/terminal.h"
#include "lib/render.h"
#include <stdlib.h>
#include <string.h>
#include "keycodes.h"

#include "components/component_form.h"

#define INPUT_MAX_LENGTH 128

void component_form(void (*header)(), Field *fields, size_t fields_length, void (*footer)())
{
    static char cursor_char = '_';
    static int focus = 0;
    static int blink = 0;
    static size_t cursor_pos = 0;

    // Flags if we should apply changes
    // when we come back to the main page
    int apply = 1;
    char *field;
    char *buffer = malloc(fields_length * sizeof(char) * INPUT_MAX_LENGTH);
    size_t *length;
    size_t *lengths = malloc(fields_length * sizeof(size_t));
    // Sets all lengths to zero as a intializer
    memset(lengths, 0, fields_length * sizeof(size_t));

    while(1)
    {
        clear_screen();
        header();
        html_p("| ");
        html_strong(1); 
        html_p("| Adição de Produtos");
        html_strong(0);
        html_p("| Preencha os campos abaixo:");
        html_p("| ");

        for (int i = 0; i < fields_length; i++)
        {
            char *buffer_ch = buffer + i * INPUT_MAX_LENGTH;
            // Field prefix for design consistenccy
            if (focus == i)
            {
                html_strong(1);
            };

            html_span("| ");
            html_span(focus == i? "> ": "  ");
            html_span(fields[i].label);
            html_span(": ");

            if (focus == i)
            {
                html_strong(0);
            };

            switch (fields[i].type)
            {
                // Inserts money amounts just like
                // modern money handling
                case MONEY_FIELD:

                    html_span("R$ ");

                    size_t j = *(lengths + i);

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
                                html_single_char(*(buffer_ch + k));
                            }

                            break;
                        
                        default:    

                            for (int k = 0; k < j - 2; k++)
                            {
                                html_single_char(*(buffer_ch + k));
                            }

                            html_single_char(',');
                            html_single_char(*(buffer_ch + j - 2));
                            html_single_char(*(buffer_ch + j - 1));

                        break;

                    };

                    if (focus == i && blink)
                    {
                        html_single_char(cursor_char);
                    };

                    break;

                case NUMBER_FIELD:
                case TEXT_FIELD:

                    for (size_t j = 0; j < *(lengths + i); j++)
                    {
                        if (focus == i && blink && j == cursor_pos)
                        {
                            html_single_char(cursor_char);
                            continue;
                        };

                        html_single_char(*(buffer_ch + j));
                    }

                    if (focus == i && blink && *(lengths + i) == cursor_pos)
                    {
                        html_single_char(cursor_char);
                    };

                    break;
            };

            html_br();
        }

        html_p("| ");
        html_p("| ESC+S: Salvar");
        html_p("| ESC+BACK: Voltar");

        footer();

        int ch = getch(); 
        // Blinking logic
        blink = ch == -1? blink? 0: 1: 1;
        // Focus offsetting
        field = buffer + focus * INPUT_MAX_LENGTH;
        length = lengths + focus;

        static int printable_and_allowed;
        printable_and_allowed = 0;

        switch (fields[focus].type)
        {
            case TEXT_FIELD:

                if (
                    '0' <= ch && ch <= '9' ||
                    'a' <= ch && ch <= 'z' || 
                    'A' <= ch && ch <= 'Z' ||
                    ch == ' ' || 
                    ch == '-'
                )
                {
                    printable_and_allowed = 1;
                };

                break;

            case MONEY_FIELD:

                if ('0' <= ch && ch <= '9')
                {
                    printable_and_allowed = 1;
                };

                break;

            case NUMBER_FIELD:

                if ('0' <= ch && ch <= '9')
                {
                    printable_and_allowed = 1;
                };

                break;
                
        };

        switch (fields[focus].type)
        {
            case NUMBER_FIELD:
            case TEXT_FIELD:

                if (printable_and_allowed)
                {
                    if (*length >= INPUT_MAX_LENGTH)
                    {
                        continue;
                    }
                    // Derived
                    // [E] [N] [Z] [O] [] [] []
                    // query_length 4
                    // cursor_pos 1
                    // bytes = query_length - cursor_pos;
                    memmove(field + cursor_pos + 1, field + cursor_pos, *length - cursor_pos);
                    *(field + cursor_pos++) = ch;
                    (*length)++;
                    continue;
                }

                switch (ch)
                {
                    case DELETE:

                        if (*length < 1 && cursor_pos < 1)
                        {
                            continue;
                        }

                        // Derived
                        // [E] [N] [Z] [O] [] [] []
                        // query_length 4
                        // cursor_pos 1
                        // bytes = query_length - cursor_pos;
                        memmove(field + cursor_pos - 1, field + cursor_pos, *length - cursor_pos);
                        cursor_pos--;
                        (*length)--;
                    
                        break;

                    case ARROW_LEFT:

                        if (cursor_pos > 0)
                        {
                            cursor_pos--;
                        }

                        break;

                    case ARROW_RIGHT:

                        if (cursor_pos < *length)
                        {
                            cursor_pos++;
                        }

                        break;
                }

                break;

            case MONEY_FIELD:

                if (printable_and_allowed)
                {
                    if (*length >= INPUT_MAX_LENGTH)
                    {
                        continue;
                    };

                    *(field + cursor_pos++) = ch;
                    (*length)++;
                    continue;
                };

                switch (ch)
                {
                    case DELETE:

                        if (*length < 1 && cursor_pos < 1)
                        {
                            continue;
                        };

                        cursor_pos--;
                        (*length)--;

                        break;
                };

                break;
        }

        switch (ch)
        {
            case ARROW_UP:

                // Wrap back logic
                focus = focus == 0? fields_length - 1: focus - 1;
                cursor_pos = *(lengths + focus);
                blink = 0;

                // Implement logic of window rolling to the bottom 
                // of the screen and focusing the last element of the table 
        
                break;
            
            case ENTER: 

                if (focus >= fields_length - 1)
                {
                    goto save;
                };

            case TAB:
            case ARROW_DOWN:

                // Wrap back logic
                focus = focus == fields_length - 1? 0: focus + 1;
                cursor_pos = *(lengths + focus);
                blink = 0;
                break;

            case BACK_BUTTON:
            
                
        };
    
    }
    save:

    // Insert the right amounts inside
    // the memory

    free(buffer);
    free(lengths);

};