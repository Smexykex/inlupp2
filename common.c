#include "common.h"
#include "linked_list.h"
#include "stdlib.h"
#include <stdbool.h>
#include <string.h>

bool shelf_equals(elem_t element_1, elem_t element_2)
{
  shelf_t shelf_1 = *(shelf_t *)element_1.any;
  shelf_t shelf_2 = *(shelf_t *)element_2.any;

  return strcmp(shelf_1.name, shelf_2.name);
}

// Creates an empty locations list if NULL
merch_t *create_merch(char *namn, char *beskrivning, int pris,
                      ioopm_list_t *locations)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->namn = strdup(namn), merch->beskrivning = strdup(beskrivning),
  merch->pris = pris,
  merch->locations =
      locations != NULL ? locations : ioopm_linked_list_create(shelf_equals);
  return merch;
}

// Todo: Fix memory leak when merch is in the actual database
void destroy_merch(merch_t *to_be_removed_merch)
{
  free(to_be_removed_merch->namn);
  free(to_be_removed_merch->beskrivning);
  ioopm_linked_list_destroy(to_be_removed_merch->locations);
  free(to_be_removed_merch);
}

int hash_function_void(elem_t element) { return element.int_num; }

bool ioopm_int_eq_function(elem_t a, elem_t b)
{
  return a.int_num == b.int_num;
}

bool ioopm_bool_eq_function(elem_t a, elem_t b)
{
  return a.boolean == b.boolean;
}
bool ioopm_float_eq_function(elem_t a, elem_t b)
{
  return a.float_num == b.float_num;
}

bool ioopm_str_eq_function(elem_t a, elem_t b)
{
  return strcmp(a.str, b.str) == 0;
}

int ioopm_string_sum_hash(const elem_t key1)
{
  int result = 0;
  int i = 0;

  while (key1.str[i] != '\0') {
    result += key1.str[i];
    i++;
  }
  return result;
}
