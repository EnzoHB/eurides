#include "macros.h"
#include "keycodes.h"

#include "lib/go_to.h"

#include "assets/asset_banner.h"

#include "components/inline_component_breadcrumbs.h"
#include "components/component_chooser.h"

#include "routes/+page.h"
#include "routes/order/+page.h"
#include "routes/admin/+page.h"

// Always remeber to declare static functions for private functions
// so that we do not get multiple definition error
static void header()
{
    inline_component_breadcrumbs("/");
    asset_banner();
}

static void footer() {}

int route_index()
{  
    // Outside of a function the compound literal becomes a static array
    // at compilation time. However, inside a function, this behavior does not automatically 
    // apply unless we define this variable as static. When that happens, the array becomes
    // globally available within the scope of the function and the pointer will always, regardless, point to it. 
    // This, of course, introduces vulnerabilities when mutating the array, but as we are only concerned in reading 
    // the options, this will and should not be a problem
    static char* options[] = DRAW(
        "1. Administração",
        "2. Fazer Pedido",
        "3. Documentação"
    );

    int option = component_chooser(header, options, footer);

    switch (option)
    {
        case 0:
            go_to(route_index_admin);
            break;
        case 1:
            go_to(route_index_order);
            break;
        case 2:
            // go_to(route_index_docs);
            break;
        case BACK_BUTTON:
            return 1;
    }

    return 0;
};