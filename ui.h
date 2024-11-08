#pragma once

#include "common.h"
#include "db.h"
#include "hash_table.h"

void input_merch(merch_table_t *store);

void print_item(merch_t *merch);

void print_line();

void list_db(merch_table_t *store);

void remove_item_from_db(merch_table_t *store);

void edit_db(merch_table_t *store);

void show_stock(merch_table_t *store);

void replenish_stock(merch_table_t *store);

cart_t *new_cart(cart_table_t *cart_storage, size_t id);

void cart_add(merch_table_t *store, cart_table_t *cart_storage);

void cart_remove(merch_table_t *store, cart_table_t *cart_storage);

void print_menu();

char ask_question_menu();

void event_loop();
