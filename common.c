#include <ctype.h>
#include <string.h>

#include "common.h"

int hash_function_void(elem_t element) { return element.i; }

bool int_eq_function(elem_t a, elem_t b) { return a.i == b.i; }
bool bool_eq_function(elem_t a, elem_t b) { return a.b == b.b; }
bool float_eq_function(elem_t a, elem_t b) { return a.f == b.f; }
bool str_eq_function(elem_t a, elem_t b) { return strcmp(a.s, b.s) == 0; }

int string_sum_hash(const elem_t key1)
{
  int result = 0;
  int i = 0;

  while (key1.s[i] != '\0') {
    result += key1.s[i];
    i++;
  }
  return result;
}

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
