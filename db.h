#pragma once

#include "common.h"
#include "hash_table.h"
#include "linked_list.h"
#include <stdbool.h>
#include <stdlib.h>

// value: elem_t { .p = location }
typedef ioopm_list_t location_list_t;

// key: char *item_name, value: merch_t item
typedef ioopm_hash_table_t merch_table_t;

// key: char *item_name, value: size_t quantity
typedef ioopm_hash_table_t cart_items_table_t;

// key: size_t id, value: cart_t cart
typedef ioopm_hash_table_t cart_table_t;

typedef struct location location_t;
struct location {
  char *shelf;
  size_t quantity;
};

typedef struct merch merch_t;
struct merch {
  char *name;
  char *description;
  size_t price;
  location_list_t *locations;
};

typedef struct cart cart_t;

struct cart {
  size_t id;
  cart_items_table_t *items;
};

void destroy_location(location_t *location);

bool location_equals(elem_t element_1, elem_t element_2);

merch_t *create_merch(char *name, char *description, size_t price,
                      location_list_t *locations);

void edit_merch(merch_table_t *store, char *name, merch_t new_value);

void destroy_merch(merch_t *merch);

void destroy_store(merch_table_t *store);

merch_t *add_item_to_db(merch_table_t *store, char *name, char *description,
                        size_t price);

bool is_shelf_taken(merch_table_t *store, char *shelf);

bool increase_stock(merch_table_t *store, char *merch_name, char *shelf,
                    size_t add_quantity);

size_t get_total_stock(merch_t *merch);

cart_t *create_cart(size_t id);

void destroy_cart(cart_t *cart);

void increase_cart_quantity(cart_t *cart, char *item_name, size_t quantity);

bool add_to_cart(merch_table_t *store, cart_table_t *cart_storage,
                 size_t cart_id, char *item_name, size_t quantity);

bool remove_from_cart(cart_t *cart, char *name, size_t quantity);

size_t calculate_cost(merch_table_t *store, cart_t *cart);

void checkout_cart(merch_table_t *store, cart_t *cart);
