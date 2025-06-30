#include "macros.h"
#include "keycodes.h"

#include "lib/go_to.h"

#include "assets/asset_banner.h"

#include "components/inline_component_breadcrumbs.h"
#include "components/component_chooser.h"

#include "routes/admin/products/+page.h"
#include "routes/admin/users/+page.h"


static void header()
{
    inline_component_breadcrumbs("/administração/");
    asset_banner();
}

static void footer()
{

}

int route_index_admin()
{
    static char* options[] = DRAW(
        "1. Produtos",
        "2. Usuários"
    );

    int option = component_chooser(header, options, footer);

    switch (option)
    {
        case 0:
            go_to(route_index_admin_products);
            break;
        case 1:
            go_to(route_index_admin_users);
            break;
        case BACK_BUTTON:
            return 1;
    }

    return 0;

}