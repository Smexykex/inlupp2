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

merch_t make_merch(char *name, char *desc, int price, ioopm_list_t *locations)
{
  merch_t created_item;
  created_item.namn = name;
  created_item.beskrivning = desc;
  created_item.pris = price;
  created_item.locations = locations;

  return created_item;
}

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

merch_t input_merch()
{
  char *name = ask_question_string("Namn: ");
  char *desc = ask_question_string("Beskrivning: ");
  int price = ask_question_int("Pris: ");

  ioopm_list_t *locations = ioopm_linked_list_create(shelf_equals);

  merch_t return_item = make_merch(name, desc, price, locations);
  return return_item;
}

void edit_merch(merch_t *merch)
{
  char *name_of_original = merch->namn;

  char *new_name = ask_question_string("Input new name: ");

  while (strcmp(new_name, name_of_original)) {
    free(new_name);
    new_name = ask_question_string("Input new name: ");
  }

  char *new_description = ask_question_string("Input new description: ");
  int new_price = ask_question_int("Input new price: ");

  char *confirmation =
      ask_question_string("Do you want edit %s, type 'y' for yes ");

  char *first_char = &confirmation[0];
  *first_char = tolower(confirmation[0]);

  if (strcmp(confirmation, "y")) {
    merch->namn = new_name;
    merch->beskrivning = new_description;
    merch->pris = new_price;
  }

  free(confirmation);
}

// TODO
void move_item(ioopm_hash_table_t *merch_data_base) { return; }

void edit_db(ioopm_hash_table_t *merch_data_base)
{
  char *merch_to_edit;
  elem_t *retrived_value;

  do {
    merch_to_edit = ask_question_string("Input merch name: ");
    elem_t converted_merch = {.str = merch_to_edit};
    retrived_value = ioopm_hash_table_lookup(merch_data_base, converted_merch);
  } while (retrived_value == NULL);

  free(merch_to_edit);

  merch_t *retrived_merch = retrived_value->any;
  edit_merch(retrived_merch);
  move_item(merch_data_base);
}

void add_item_to_db(ioopm_hash_table_t *merch_data_base)
{
  merch_t new_item = input_merch();

  merch_t *added_item = malloc(sizeof(merch_t));
  added_item->namn = new_item.namn;
  added_item->beskrivning = new_item.beskrivning;
  added_item->pris = new_item.pris;
  added_item->locations = new_item.locations;

  elem_t merch_key = {.str = added_item->namn};
  elem_t merch_value = {.any = added_item};
  
  ioopm_hash_table_insert(merch_data_base, merch_key, merch_value);
}

void destroy_store(ioopm_hash_table_t *store)
{
  size_t size = ioopm_hash_table_size(store);

  elem_t *items = ioopm_hash_table_values(store);
  
  for (size_t i = 0; i < size; i++)
  {
    destroy_merch((merch_t *) items[i].any);
  }
  
  ioopm_hash_table_destroy(store);
  free(items);
}

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
