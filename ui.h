#pragma once

#include "common.h"
#include "hash_table.h"

void input_merch(ioopm_hash_table_t *store);

void print_item(merch_t *merch);

void print_line();

void list_db(ioopm_hash_table_t *store);

void edit_db(ioopm_hash_table_t *store);

void print_menu();

char ask_question_menu();

void event_loop();
