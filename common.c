#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"

bool is_valid_shelf(char *shelf)
{
  if (!isalpha(*shelf)) {
    return false;
  }
  ++shelf;
  while (*shelf != '\0') {
    if (!isdigit(*shelf)) {
      return false;
    }
    ++shelf;
  }
  return true;
}

bool shelf_equals(elem_t element_1, elem_t element_2)
{
  location_t shelf_1 = *(location_t *)element_1.any;
  location_t shelf_2 = *(location_t *)element_2.any;

  return strcmp(shelf_1.shelf, shelf_2.shelf) == 0;
}

int hash_function_void(elem_t element) { return element.i; }

bool ioopm_int_eq_function(elem_t a, elem_t b) { return a.i == b.i; }

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
