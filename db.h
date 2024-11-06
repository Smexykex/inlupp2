#pragma once

#include "common.h"
#include "hash_table.h"
#include <stdbool.h>

merch_t *create_merch(char *namn, char *beskrivning, int pris,
                      ioopm_list_t *locations);

void edit_merch(ioopm_hash_table_t *store, char *name, merch_t new_value);

void destroy_merch(merch_t *to_be_removed_merch);

void destroy_store(ioopm_hash_table_t *store);

void add_item_to_db(ioopm_hash_table_t *store, char *name, char *description,
                    int price);

bool is_shelf_taken(ioopm_hash_table_t *store, char *shelf);

bool increase_stock(ioopm_hash_table_t *store, char *merch_name, char *shelf,
                    int add_quantity);

void create_cart(ioopm_hash_table_t *cart_storage);

void remove_cart(ioopm_hash_table_t *cart_storage);

void add_to_cart(ioopm_hash_table_t *cart_storage);

void remove_from_cart(ioopm_hash_table_t *cart_storage);

int calculate_cost(ioopm_hash_table_t *cart_storage);

void checkout(ioopm_hash_table_t *cart_storage);
