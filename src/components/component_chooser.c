#include <stdio.h>

#include <keycodes.h>

#include "lib/render.h"
#include "lib/terminal.h"

#include "components/component_chooser.h"

int component_chooser(void (*header)(), char* options[], void (*footer)())
{
    int first_time = 0;
    int length = 0;
    int selected = 0;
    int blink = 0;

    while(1)
    {
        clear_screen();
        header();

        for (int i = 0; options[i] != NULL; i++)
        {
            if (first_time == 0)
            {
                length++;
            }

            if (selected == i)
            {
                strong(1);
                    span(blink? "> ": "  ");
                    span(options[i]); 
                    br();
                strong(0);
                continue;
            }

            span("  ");
            span(options[i]);
            br();
        };

        int ch = getch();

        switch (ch)
        {
            case ARROW_UP: 

                // Wrap back to the end
                selected--;
                if (selected == -1)
                {
                    selected = length - 1;
                };
                blink = 1;
                break;

            case ARROW_DOWN:

                // Wrap back to the beginning
                selected++;
                if (selected == length)
                {
                    selected = 0;
                }
                blink = 1;
                break;

            case NONE:

                if (blink == 1)
                {
                    blink--;
                    continue;
                }
                
                blink++;
                break;

            case ENTER:
                return selected;

            case BACK_BUTTON:
                return ch;
        }

        first_time = 1;
    }
};