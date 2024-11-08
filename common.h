#pragma once

#include <stdbool.h>
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

// an entry in a linked list
typedef struct list_entry list_entry_t;
typedef struct list_entry {
  elem_t value;       // then entry's value
  list_entry_t *next; // a pointer to the next entry in the list
} list_entry_t;

// a linked list
typedef struct list {
  list_entry_t *first; // pointer to the first element in the list
  list_entry_t *last;  // pointer to the last element in the list
  int size;            // the size of the list
  ioopm_eq_function
      *eq_func; // pointer to a function that compares two elements in the list
} ioopm_list_t;

typedef struct ioopm_list_iterator {
  list_entry_t *current; // the current element
  ioopm_list_t *list;    // The underlying list
} ioopm_list_iterator_t;

typedef struct merch merch_t;
struct merch {
  char *name;
  char *description;
  int price;
  ioopm_list_t *locations;
};

typedef struct location location_t;
struct location {
  char *shelf;
  size_t quantity;
};

typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);

bool int_eq_function(elem_t a, elem_t b);
bool float_eq_function(elem_t a, elem_t b);
bool bool_eq_function(elem_t a, elem_t b);
bool str_eq_function(elem_t a, elem_t b);

int hash_function_void(elem_t element);

int string_sum_hash(const elem_t key1);

bool is_valid_shelf(char *shelf);

bool shelf_equals(elem_t element_1, elem_t element_2);
