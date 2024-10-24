#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "db.h"
#include "linked_list.h"
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

// Creates an empty locations list if NULL
merch_t *create_merch(char *namn, char *beskrivning, int pris,
                      ioopm_list_t *locations)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->namn = strdup(namn);
  merch->beskrivning = strdup(beskrivning);
  merch->pris = pris;
  merch->locations =
      locations != NULL ? locations : ioopm_linked_list_create(shelf_equals);
  return merch;
}

// does not edit locations
void edit_merch(ioopm_hash_table_t *store, char *name, merch_t new_value)
{
  merch_t *item =
      (merch_t *)ioopm_hash_table_lookup(store, str_elem(name))->any;
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

void destroy_merch(merch_t *to_be_removed_merch)
{
  free(to_be_removed_merch->namn);
  free(to_be_removed_merch->beskrivning);
  ioopm_linked_list_destroy(to_be_removed_merch->locations);
  free(to_be_removed_merch);
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

void add_item_to_db(ioopm_hash_table_t *store, char *name, char *description,
                    int price)
{
  merch_t *merch = create_merch(name, description, price, NULL);
  ioopm_hash_table_insert(store, str_elem(merch->namn), p_elem(merch));
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
