#pragma once

#include "common.h"
#include "hash_table.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct cart cart_t;

struct cart {
  size_t id;
  ioopm_hash_table_t *items; // key: char *item_name, value: size_t quantity
};

merch_t *create_merch(char *name, char *description, int price,
                      ioopm_list_t *locations);

void edit_merch(ioopm_hash_table_t *store, char *name, merch_t new_value);

void destroy_location(location_t *location);

void destroy_merch(merch_t *merch);

void destroy_store(ioopm_hash_table_t *store);

merch_t *add_item_to_db(ioopm_hash_table_t *store, char *name,
                        char *description, int price);

bool is_shelf_taken(ioopm_hash_table_t *store, char *shelf);

bool increase_stock(ioopm_hash_table_t *store, char *merch_name, char *shelf,
                    size_t add_quantity);

size_t get_total_stock(merch_t *merch);

cart_t *create_cart(size_t id);

void destroy_cart(cart_t *cart);

void increase_cart_quantity(cart_t *cart, char *item_name, size_t quantity);

bool add_to_cart(ioopm_hash_table_t *store, ioopm_hash_table_t *carts,
                 size_t cart_id, char *item_name, size_t quantity);

bool remove_from_cart(cart_t *cart, char *name, size_t quantity);

size_t calculate_cost(ioopm_hash_table_t *store, cart_t *cart);

void checkout_cart(ioopm_hash_table_t *store, cart_t *cart);
