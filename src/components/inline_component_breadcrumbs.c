#include "lib/render.h"

void inline_component_breadcrumbs(char breadcrumbs[])
{
    html_span("Caminho: ");
    html_span(breadcrumbs);
    html_br();
}