#include "lib/render.h"
#include "macros.h"

void asset_banner() 
{
    static char *body[] = DRAW(
        "-----------------Mickey & Donald-----------------"
    );
    
    render(body);
}