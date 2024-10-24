#include "common.h"
#include <stdbool.h>
#include <string.h>

bool shelf_equals(elem_t element_1, elem_t element_2)
{
  shelf_t shelf_1 = *(shelf_t *)element_1.any;
  shelf_t shelf_2 = *(shelf_t *)element_2.any;

  return strcmp(shelf_1.name, shelf_2.name);
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
