#include "lib/render.h"

void component_breadcrumbs(char breadcrumbs[])
{
    span("Caminho: ");
    span(breadcrumbs);
    br();
}