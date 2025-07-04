#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "lib/terminal.h"
#include "lib/render.h"
#include "keycodes.h"
#include "lib/utils.h"

#include "components/component_form.h"

#define INPUT_MAX_LENGTH 128

void component_form(void (*header)(), Field *fields, size_t fields_length, char* title, void (*footer)())
{
    static char cursor_char = '_';
    int focus = 0;
    int blink = 0;
    int cursor_pos = 0;
    // Flags if we should apply changes
    // when we come back to the main page
    int apply = 1;
    char *field;
    char *buffer = malloc(fields_length * sizeof(char) * INPUT_MAX_LENGTH);
    int *length;
    int *lengths = calloc(fields_length, sizeof(int));

    while(1)
    {
        clear_screen();
        header();
        html_p("| ");
        html_strong(1); 
        html_span("| Adição de ");
        html_span(title);
        html_br();
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
            html_span(focus == i? ">> ": "   ");
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

                    _html_currency(buffer_ch, *(lengths + i));

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
            case NUMBER_FIELD:
            case MONEY_FIELD:

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
            {
                input_text_controller(ch, field, length, &cursor_pos, INPUT_MAX_LENGTH);
                break;
            }
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

                // TODO: Implement logic of window rolling to the bottom 
                // of the screen and focusing the last element of the table 
        
                break;
            
            case ESCAPE_MODE_OFFSET + KEY_S:
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
                
                apply = 0;
                goto save;
                
        };
    
    }

    save:

    if (apply)
    {
        for (int i = 0; i < fields_length; i++)
        {
            field = buffer + i * INPUT_MAX_LENGTH;
            length = lengths + i;

            switch (fields[i].type)
            {
                case TEXT_FIELD:
                    
                    for (int k = 0; k < *length; k++)
                    {
                        *((char *)fields[i].field + k) = *(field + k);
                    };

                    *((char *)fields[i].field + *length) = '\0';
                    break;
                
                case MONEY_FIELD:
                case NUMBER_FIELD:

                    int value = 0;

                    for (int k = 0; k < *length; k++)
                    {
                        value = value * 10 + *(field + k) - '0';
                    };

                    *((int *)fields[i].field) = value;
                    break;
            }
        }
    }

    free(buffer);
    free(lengths);
};