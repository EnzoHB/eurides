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
#include "components/component_form.h"

#include "routes/admin/products/+page.h"

static void header()
{
    inline_component_breadcrumbs("/administração/produtos/");
    asset_banner();
}

static void footer()
{
   
}

#define INPUT_CHAR_LIMIT 128
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

};


int route_index_admin_products()
{
    static char cursor = '_'; // '█';

    setup_products();   
    setup_cells();

    int focus = 0;

    // Maybe transform this in a field
    char query[INPUT_CHAR_LIMIT];
    size_t query_length = 0;
    size_t cursor_pos = 0;
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
            if (focus == SEARCH_FOCUSED && blink && i == cursor_pos)
            {
                html_single_char(cursor);
                continue;
            }

            html_single_char(query[i]);
        };

        if (focus == SEARCH_FOCUSED && blink && query_length == cursor_pos)
        {
            html_single_char(cursor);
        };

        html_br();

        html_strong(1);
        html_p("Ações:");
        html_strong(0);

        html_span(focus == BUTTON_FOCUSED && blink? "> ": "  ");
        html_span("Novo Produto"); 
        html_br();

        html_strong(1);
        html_p("Produtos:");
        html_strong(0);
        

        html_br();



        // Botão

        // Tabela




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

                // Typing logic
                if (query_length < INPUT_CHAR_LIMIT && (
                    'a' <= ch && ch <= 'z' || 
                    'A' <= ch && ch <= 'Z' || 
                    ch == ' '))
                {
                    // Derived
                    // [E] [N] [Z] [O] [] [] []
                    // query_length 4
                    // cursor_pos 1
                    // bytes = query_length - cursor_pos;
                    memmove(&query[cursor_pos + 1], &query[cursor_pos], query_length - cursor_pos);
                    query[cursor_pos++] = ch;
                    query_length++;
                    continue;
                };

                switch (ch)
                {
                    case DELETE:

                        if (query_length < 1 && cursor_pos < 1)
                        {
                            continue;
                        }

                        // Derived
                        // [E] [N] [Z] [O] [] [] []
                        // query_length 4
                        // cursor_pos 1
                        // bytes = query_length - cursor_pos;
                        memmove(&query[cursor_pos - 1], &query[cursor_pos], query_length - cursor_pos);
                        cursor_pos--;
                        query_length--;
                        
                        break;

                    case ARROW_UP:

                        // Implement logic of window rolling to the bottom 
                        // of the screen and focusing the last element of the table 
                        focus--;
                        break;
                    
                    case ARROW_DOWN:
                        focus++;
                        break;

                    case ARROW_LEFT:

                        if (cursor_pos > 0)
                        {
                            cursor_pos--;
                        }

                        break;

                    case ARROW_RIGHT:

                        if (cursor_pos < query_length)
                        {
                            cursor_pos++;
                        }

                        break;
                };

                break;
            
            case BUTTON_FOCUSED:

                switch (ch)
                {
                    case ENTER:

                        Product product;
                        Field fields[] = {
                            { "Nome", &product.label, TEXT_FIELD },
                            { "Preço", &product.price, MONEY_FIELD }
                        };

                        // Always use sizeof(<local_array>[0]) because this will
                        // allow for both extensions and changes in typing in the future
                        component_form(header, fields, sizeof(fields) / sizeof(fields[0]), footer);
                        // products has to receive products.length
                        vector_push(&products, (void *)&product);
                }

                break;

            case TABLE_FOCUSED:

            default:
                break;
        }
    }
    // Remmeber to return 1 for the back button
    return 0;
};