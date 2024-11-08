#pragma once

#include "common.h"
#include "db.h"
#include "hash_table.h"

void input_merch(ioopm_hash_table_t *store);

void print_item(merch_t *merch);

void print_line();

void list_db(ioopm_hash_table_t *store);

void remove_item_from_db(ioopm_hash_table_t *merch_data_base);

void edit_db(ioopm_hash_table_t *store);

void show_stock(ioopm_hash_table_t *store);

void replenish_stock(ioopm_hash_table_t *store);

cart_t *new_cart(ioopm_hash_table_t *cart_storage, int id);

void cart_add(ioopm_hash_table_t *store, ioopm_hash_table_t *cart_storage);

void cart_remove(ioopm_hash_table_t *store, ioopm_hash_table_t *cart_storage);

void print_menu();

char ask_question_menu();

void event_loop();
