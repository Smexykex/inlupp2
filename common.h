#pragma once

#include <stdlib.h>

typedef union elem elem_t;
union elem {
  int i;
  size_t u;
  bool b;
  float f;
  void *p;
  char *s;
};

#define s_elem(x)                                                              \
  (elem_t) { .s = x }
#define p_elem(x)                                                              \
  (elem_t) { .p = x }
#define i_elem(x)                                                              \
  (elem_t) { .i = x }
#define u_elem(x)                                                              \
  (elem_t) { .u = x }

typedef bool ioopm_eq_function(elem_t a, elem_t b);

typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);

bool int_eq_function(elem_t a, elem_t b);
bool float_eq_function(elem_t a, elem_t b);
bool bool_eq_function(elem_t a, elem_t b);
bool str_eq_function(elem_t a, elem_t b);

int hash_function_void(elem_t element);

int string_sum_hash(const elem_t key1);

bool is_valid_shelf(char *shelf);
