#include "macros.h"
#include "keycodes.h"

#include "lib/io.h"

#include "assets/asset_banner.h"

#include "components/component_breadcrumbs.h"
#include "components/component_chooser.h"

#include "routes/admin/products/+page.h"

static void header()
{
    component_breadcrumbs("/administração/");
    asset_banner();
}

typedef struct Product {
    int id;
    char name[128];
    int price;
} Product;

static char path[] = "../../../../data/products.dat"; 

static void footer()
{
    size_t length;
    Product* products = (Product*)alloc_structs_from_file(path, &length); // Pas the sieze of the struct

    // Render fallback if products pointer is null

    // char** 
    // An array of an array of strings, each is going to receive a row
    // Maybe first I have to create a table api that will allow me to declare 
    // These things at ccompilation time. 

    for (size_t i = 0; i < length; i++)
    {
        Product product = products[i];
    }
    // We have to convert products into a struct that can be understood by the table tag
    // We have to passing a null terminated array of null terminated arrays all as 
    // Here we need to render all products from products .dat in a table like manner

    // TODO: Create a function that abstracts this routine
    //
}

int route_index_admin_products()
{
    static char* options[] = DRAW(
        "1. Editar",
        "2. Popular",
        "3. Limpar"
    );

    int option = component_chooser(header, options, footer);

    switch (option)
    {
        case 0:

            //Edit
            break;
            
        case 1:

            // Populate
            break;
        
        case 2:
    
            // Clear
            break;

        case BACK_BUTTON:
            return 1;
    }

    return 0;

}
// IMplement the products page
// Implement the table tag
// implement the form component
// Implement local storage?
