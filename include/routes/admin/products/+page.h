#ifndef INDEX_ADMIN_PRODUCTS_H
#define INDEX_ADMIN_PRODUCTS_H

typedef struct Product {
    int id;
    int price;
    char label[128];
} Product;

int route_index_admin_products(void);

#endif