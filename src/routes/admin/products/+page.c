#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "keycodes.h"

#include "lib/terminal.h"
#include "lib/render.h"
#include "lib/vector.h"
#include "lib/utils.h"

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

#define QUERY_MAX_LENGTH 128
#define SEARCH_FOCUSED 0
#define BUTTON_FOCUSED 1
#define TABLE_FOCUSED 2

#define ACTION_NEW 0

static Vector products;
static char path[] = "../../../../data/products.dat";
static int products_initialized = 0;
static Cell *cells;
static int cells_initialized = 0;
static int rows;
static int columns = 4;

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
    // We have to make sure that products is intialized
    // before we call setup_cells();
    if (cells_initialized || !products_initialized)
    {
        return;
    }

    // Header
    rows = products.length + 1;
    cells = malloc(sizeof(Cell) * columns * rows);

    if (cells == NULL)
    {
        exit(1);
    };

    cells_initialized++;
};


int route_index_admin_products()
{
    static char cursor_character = '_'; // '█';
    setup_products();   
    setup_cells();
    int focus = 0;
    int blink = 0;
    // Search support TODO: vectorize this
    char query[128];
    int query_length = 0;
    int cursor_pos = 0;
    // Actions
    int action_selected = 0;
    // Table support
    int item_selected = 0;

    // We need to abstract this logic to allow for the coupling of fields
    while(1)
    {
        clear_screen();
        header();

        html_strong(1);
        html_p("Pesquisar:");
        html_strong(0);

        if (focus == SEARCH_FOCUSED)
        {
            html_span(">> ");

            for (int i = 0; i < query_length; i++)
            {
                if (blink && i == cursor_pos)
                {
                    html_single_char(cursor_character);
                    continue;
                }

                html_single_char(query[i]);
            };

            if (blink && query_length == cursor_pos)
            {
                html_single_char(cursor_character);
            };
        }
        else 
        {
            html_span("   ");
            html_span(query_length? query: "");
        };
        
        html_br();
        html_strong(1);
        html_p("Ações:");
        html_strong(0);

        if (focus == BUTTON_FOCUSED)
        {
            html_p("> Novo Produto");
        }
        else 
        {
            html_p("  Novo Produto");
        }

        html_strong(1);
        printf("Tabela (%zu):", products.length);
        html_br();
        html_strong(0);

        if (rows != products.length + 1)
        {
            void *p = realloc(cells, sizeof(Cell) * columns * (products.length + 1));

            if (p == NULL)
            {
                exit(0);
                return 1;
            }

            cells = p;
            rows = products.length + 1;
        };

        static const size_t c1_length = 1;
        static const size_t c2_length = 3;
        static const size_t c3_length = 28;
        static const size_t c4_length = 12;
        static Cell h1 = { 1, ALIGN_LEFT, c1_length, "#" };
        static Cell h2 = { 1, ALIGN_LEFT, c2_length, "ID" };  
        static Cell h3 = { 1, ALIGN_LEFT, c3_length, "Nome" };
        static Cell h4 = { 1, ALIGN_LEFT, c4_length, "Valor" };;
        cells[0 + 0] = h1;;
        cells[0 + 1] = h2;
        cells[0 + 2] = h3;
        cells[0 + 3] = h4;

        for (int j = 0; j < products.length; j++)
        {
            Product *product = &((Product *)products.elements)[j];
            int bold = item_selected == j? 1 : 0;
            char id[32] ;
            char price[32];
            char pretty_price[32];
            int_to_numeral(product->id, id, sizeof(id));
            int_to_numeral(product->price, price, sizeof(price));
            format_price(price, pretty_price);
            // We  have to use this intialization strategy or else the buffer
            // gets overwritten on each subsequent loop iteration. The downside
            // of this approach is that we limit the maximum number of characters
            // to the size of the internal buffer of Cell
            // This of course could have been mitigated by dinamically allocating memory for this operation
            // however, I neither have the time nor the patience to debug the errors of this
            Cell *c1 = &cells[(j + 1) * columns + 0];
            Cell *c2 = &cells[(j + 1) * columns + 1];
            Cell *c3 = &cells[(j + 1) * columns + 2];
            Cell *c4 = &cells[(j + 1) * columns + 3];
            c1->bold = bold;
            c1->align = ALIGN_LEFT;
            c1->length = c1_length;
            snprintf(c1->text, sizeof(c1->text), "%s", focus == TABLE_FOCUSED && bold ? ">" : " ");
            c2->bold = bold;
            c2->align = ALIGN_RIGHT;
            c2->length = c2_length;
            snprintf(c2->text, sizeof(c2->text), "%s", id);
            c3->bold = bold;
            c3->align = ALIGN_LEFT;
            c3->length = c3_length;
            snprintf(c3->text, sizeof(c3->text), "%s", product->label);
            c4->bold = bold;
            c4->align = ALIGN_LEFT;
            c4->length = c4_length;
            snprintf(c4->text, sizeof(c4->text), "%s", pretty_price);
        }

        inline_component_table(cells, columns, rows);
        int ch = getch();
        // Blinking logic
        blink = ch == -1? blink? 0: 1: 1;

        switch (focus)
        {
            case SEARCH_FOCUSED:
            {
                switch (ch)
                {
                    case ARROW_DOWN:
                    {
                        focus++;
                        continue;
                    }
                }

                input_text_controller(ch, (char *)&query, &query_length, &cursor_pos, QUERY_MAX_LENGTH);
                break;
            }
            case BUTTON_FOCUSED:
            {
                switch (ch)
                {
                    case ENTER:
                    {
                        switch (item_selected)
                        {
                            case ACTION_NEW:
                            {
                                Product product = {};
                                Field fields[] = {
                                    { "Nome", &product.label, TEXT_FIELD },
                                    { "Preço", &product.price, MONEY_FIELD }
                                };
                                // Always use sizeof(<local_array>[0]) because this will
                                // allow for both extensions and changes in typing in the future
                                component_form(header, fields, sizeof(fields) / sizeof(fields[0]),"Produtos", footer);
                                product.id = products.length + 1;
                                // products has to receive products.length
                                vector_push(&products, (void *)&product);
                                continue;
                            }
                        }
                    }
                    case ARROW_DOWN:
                    {
                        // item_selected + 1 >= qtd_actions
                        if (action_selected + 1 >= 1)
                        {
                            focus++;
                            continue;
                        }

                        action_selected++;
                    }
                    case ARROW_UP:
                    {
                        if (action_selected - 1 < 0)
                        {
                            focus--;
                            continue;
                        }

                        action_selected--;
                    }
                }
                break;
            };
            case TABLE_FOCUSED:
            {
                switch (ch)
                {
                    case ENTER:
                    {

                    }
                    case ARROW_DOWN:
                    {
                        // item_selected + 1 >= qtd_actions
                        if (item_selected + 1 >= products.length)
                        {
                            continue;
                        }

                        item_selected++;
                    }
                    case ARROW_UP:
                    {
                        if (item_selected - 1 < 0)
                        {
                            focus--;
                            continue;
                        }

                        item_selected--;
                    }
                }
            }
        }
    }
    // Remmeber to return 1 for the back button
    return 0;
};