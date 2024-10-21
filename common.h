#pragma once

#include <stdbool.h>
#include "hash_table.h"
#include "linked_list.h"

struct merch
{
  char *namn;
  char *beskrivning;
  int pris;
  ioopm_list_t *locations;
};

typedef struct merch merch_t;

struct shelf
{
  char *name;
  int no_items;
};

typedef struct shelf shelf_t;

bool shelf_equals(elem_t element_1, elem_t element_2);

// Todo: Fix memory leak when merch is in the actual database
void destroy_merch(merch_t *to_be_removed_merch);

int hash_function_void(elem_t element);