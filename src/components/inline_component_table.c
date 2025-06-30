#include <string.h>
#include <stdlib.h>
#include "components/inline_component_table.h"
#include "lib/render.h"

static void white_space(size_t times)
{
    for (int i = 0; i < times; i++)
    {
        html_span(" ");
    };
}

void inline_component_table(Cell **cells, int width, int height)
{
    if (width == 0)
    {
        html_p("<Render Error>: Cannot have zero width table");
        return;
    }

    if (height == 0)
    {
        html_p("<Render Error>: Cannot have zero height table");   
        return;
    }

    // We have to account for border in between cells
    // ----------------------------------------
    // | Something | Something | Something |
    int line_width = width + 1; 
    // Calculating the line width of the rendered table
    // while concurrently rendering it;
    html_span("-");

    for (int i = 0; i < width; i++)
    {
        int length = cells[0][i].length; 

        // length + 1 is the amount of characters 
        // until the top-left border of the next cell
        for (int k = 0; k < length + 1; k++)
        {
            html_span("-");
        }

        line_width += length;
    };

    html_br();

    for (int j = 0; j < height; j++)
    {
        html_span("|");

        for (int i = 0; i < width; i++, html_span("|"))
        {
            Cell cell = cells[j][i];
            size_t text_length = strlen(cell.text);

            if (text_length < cell.length)
            {
                size_t white_space_padding = cell.length - text_length;

                if (cell.align == ALIGN_LEFT)
                {
                    html_span(cell.text);
                    white_space(white_space_padding);
                    continue;
                }
                else if (cell.align == ALIGN_RIGHT) 
                {
                    white_space(white_space_padding);
                    html_span(cell.text);
                    continue;
                }
            }

            if (text_length == cell.length)
            {
                html_span(cell.text);
                continue;
            }

            for (size_t i = 0; i < text_length - 3; i++)
            {
                // We had to create this function otherwise spa would onsume the
                // entirety of the string on its first call, which is not the desired behavior
                html_single_char(cell.text[i]);
            }

            html_span("...");
        }
    };

    // Render the line altogether
    for (int i = 0; i < line_width; i++)
    {
        html_span("-");
    }
}