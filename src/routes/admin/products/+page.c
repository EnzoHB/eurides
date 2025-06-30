#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "keycodes.h"

#include "lib/terminal.h"
#include "lib/render.h"
#include "lib/vector.h"

#include "assets/asset_banner.h"

#include "components/inline_component_table.h"
#include "components/inline_component_breadcrumbs.h"
#include "components/component_chooser.h"

#include "routes/admin/products/+page.h"

static void header()
{
    inline_component_breadcrumbs("/administração/produtos/");
    asset_banner();
}

static void footer()
{
   
}

#define SEARCH_FOCUSED 0
#define BUTTON_FOCUSED 1
#define TABLE_FOCUSED 2

static Vector cells;
static Vector products;

static char path[] = "../../../../data/products.dat";
static int products_initialized = 0;
static int cells_initialized = 0;

static void setup_products()
{
    if (products_initialized)
    {
        return;
    };

    // Here we should test if we have allocated memory
    vector_init(&products, sizeof(Product));
    vector_populate_from_file(&products, path);
    products_initialized++;

};

static void setup_cells()
{
    if (cells_initialized)
    {
        return;
    }

    void *p = malloc(sizeof(Cell));

    free(p);

}

int route_index_admin_products()
{
    static char cursor = '_'; // '█';

    setup_products();   
    setup_cells();

    int focus = 0;

    // Maybe transform this in a field
    char query[128] = "";
    size_t query_length = 0;
    size_t cursor_column = 1;
    int blink = 0;

    static size_t header_height;
    
    // We need to abstract this logic to allow for the coupling of fields
    while(1)
    {
        clear_screen();
        header();

        html_strong(1);
            html_span("Pesquisar: ");
        html_strong(0);

        for (int i = 0; i < query_length; i++)
        {
            if (blink && i + 1 == cursor_column)
            {
                html_single_char(cursor);
                continue;
            }

            html_single_char(query[i]);
        };

        if (blink && query_length + 1 == cursor_column)
        {
            html_single_char(cursor);
        };

        html_br();

        int ch = getch();

        // Blinking logic
        if (ch == -1)
        {
            blink += blink? -1: 1;    
        }
        else 
        {
            blink = 1;
        }

        switch (focus)
        {
            case SEARCH_FOCUSED:

                if (query_length > 127)
                {
                    continue;
                }

                // Typing logic
                if ('a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == ' ')
                {
                    query[query_length++] = ch;
                    query[query_length] = '\0';
                    cursor_column++;
                    continue;
                };

                switch (ch)
                {
                    case DELETE:

                        if (!query_length)
                        {
                            continue;
                        }

                        query[--query_length] = '\0';
                        cursor_column--;
                        break;

                    case ARROW_UP:

                        // Implement logic of window rolling to the bottom 
                        // of the screen and focusing the last element of the table 
                        break;
                    
                    case ARROW_DOWN:
                        focus++;
                        break;

                    case ARROW_LEFT:

                        if (cursor_column > 1)
                        {
                            cursor_column--;
                        }

                        break;

                    case ARROW_RIGHT:

                        if (cursor_column < 128)
                        {
                            cursor_column++;
                        }

                        break;
                };

                break;
            
            case BUTTON_FOCUSED:

                

                break;

            case TABLE_FOCUSED:

            default:
                break;
        }
    }
    // Remmeber to return 1 for the back button
    return 0;
};