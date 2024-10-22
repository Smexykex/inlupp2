#pragma once

#include "common.h"
#include "hash_table.h"
#include <stdbool.h>

void edit_merch(ioopm_hash_table_t *store, char *name, merch_t new_value);

void add_item_to_db(ioopm_hash_table_t *store, char *name, char *description,
                    int price);

void remove_item_from_db(ioopm_hash_table_t *merch_data_base);

void show_stock(ioopm_hash_table_t *merch_data_base);

void replenish_stock(ioopm_hash_table_t *merch_data_base);

void create_cart(ioopm_hash_table_t *cart_storage);

void remove_cart(ioopm_hash_table_t *cart_storage);

void add_to_cart(ioopm_hash_table_t *cart_storage);

void remove_from_cart(ioopm_hash_table_t *cart_storage);

int calculate_cost(ioopm_hash_table_t *cart_storage);

void checkout(ioopm_hash_table_t *cart_storage);

// Todo: Fix memory leak when merch is in the actual database
void destroy_merch(merch_t *to_be_removed_merch);

void destroy_store(ioopm_hash_table_t *store);
