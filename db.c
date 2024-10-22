#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "db.h"
#include "utils.h"

extern char *strdup(const char *);

bool is_item(char *shelf)
{
  char *copy_of_shelf = string_lower(shelf);
  if (!(copy_of_shelf[0] > 96 && copy_of_shelf[0] < 123)) {
    return false;
  } else {
    bool is_remainder_a_number = is_number(copy_of_shelf + 1);
    free(copy_of_shelf);
    return is_remainder_a_number;
  }
}

char *ask_question_shelf(char *question)
{
  answer_t ans = ask_question(question, is_item, (convert_func *)strdup);
  char *string_answer = ans.string_value;
  string_answer[0] = toupper(string_answer[0]);
  return string_answer;
}

// does not edit locations
void edit_merch(ioopm_hash_table_t *store, char *name, merch_t new_value)
{
  merch_t *item =
      (merch_t *)ioopm_hash_table_lookup(store, (elem_t){.str = name})->any;
  assert(item != NULL);

  ioopm_hash_table_remove(store, str_elem(name));

  free(item->namn);
  free(item->beskrivning);
  item->namn = strdup(new_value.namn);
  item->beskrivning = strdup(new_value.beskrivning);
  item->pris = new_value.pris;
  item->locations = item->locations;

  ioopm_hash_table_insert(store, str_elem(item->namn), p_elem(item));
}

void add_item_to_db(ioopm_hash_table_t *store, char *name, char *description,
                    int price)
{
  merch_t *merch = create_merch(name, description, price, NULL);
  ioopm_hash_table_insert(store, str_elem(merch->namn), p_elem(merch));
}

void destroy_store(ioopm_hash_table_t *store)
{
  size_t size = ioopm_hash_table_size(store);

  elem_t *items = ioopm_hash_table_values(store);

  for (size_t i = 0; i < size; i++) {
    destroy_merch((merch_t *)items[i].any);
  }

  ioopm_hash_table_destroy(store);
  free(items);
}

// TODO move to ui.c
void remove_item_from_db(ioopm_hash_table_t *merch_data_base)
{
  char *merch_to_remove;
  elem_t *retrived_value;

  do {
    merch_to_remove = ask_question_string("Input merch name: ");
    retrived_value = ioopm_hash_table_lookup(merch_data_base,
                                             (elem_t){.str = merch_to_remove});
  } while (retrived_value == NULL);

  char *confirmation =
      ask_question_string("Do you want delete %s, type 'y' for yes ");

  char *first_char = &confirmation[0];
  *first_char = tolower(confirmation[0]);

  if (strcmp(confirmation, "y")) {
    merch_t *retrived_merch = retrived_value->any;
    destroy_merch(retrived_merch);
  }

  free(merch_to_remove);
}

// TODO
void show_stock(ioopm_hash_table_t *db) { puts("Not yet implemented!"); }

// TODO
void replenish_stock(ioopm_hash_table_t *db) { puts("Not yet implemented!"); }

// TODO
void create_cart(ioopm_hash_table_t *cart_storage)
{
  puts("Not yet implemented!");
}

// TODO
void remove_cart(ioopm_hash_table_t *cart_storage)
{
  puts("Not yet implemented!");
}

// TODO
void add_to_cart(ioopm_hash_table_t *cart_storage)
{
  puts("Not yet implemented!");
}

// TODO
void remove_from_cart(ioopm_hash_table_t *cart_storage)
{
  puts("Not yet implemented!");
}

// TODO
int calculate_cost(ioopm_hash_table_t *cart_storage)
{
  puts("Not yet implemented!");
  return -1;
}

// TODO
void checkout(ioopm_hash_table_t *cart_storage)
{
  puts("Not yet implemented!");
}
