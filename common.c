#include <stdbool.h>
#include <string.h>
#include "common.h"

bool shelf_equals(elem_t element_1, elem_t element_2)
{
  shelf_t shelf_1 = *(shelf_t *) element_1.any;
  shelf_t shelf_2 = *(shelf_t *) element_2.any;

  return strcmp(shelf_1.name, shelf_2.name);
}

// Todo: Fix memory leak when merch is in the actual database
void destroy_merch(merch_t *to_be_removed_merch)
{
  free(to_be_removed_merch->namn);
  free(to_be_removed_merch->beskrivning);
  ioopm_linked_list_destroy(to_be_removed_merch->locations);
}

int hash_function_void(elem_t element)
{
  return (int) element.any;
}