#pragma once

#include <stdbool.h>

typedef union elem elem_t;
union elem {
  int int_num;
  unsigned int unsigned_num;
  bool boolean;
  float float_num;
  void *any;
  char *str;
};

typedef bool ioopm_eq_function(elem_t a, elem_t b);

bool ioopm_int_eq_function(elem_t a, elem_t b);
bool ioopm_float_eq_function(elem_t a, elem_t b);
bool ioopm_bool_eq_function(elem_t a, elem_t b);
bool ioopm_str_eq_function(elem_t a, elem_t b);

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

struct merch {
  char *namn;
  char *beskrivning;
  int pris;
  ioopm_list_t *locations;
};

typedef struct merch merch_t;

struct shelf {
  char *name;
  int no_items;
};

typedef struct shelf shelf_t;

/// Compares two elements and returns true if they are equal
typedef bool ioopm_eq_function(elem_t a, elem_t b);

typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);

bool shelf_equals(elem_t element_1, elem_t element_2);

// Todo: Fix memory leak when merch is in the actual database
void destroy_merch(merch_t *to_be_removed_merch);

int hash_function_void(elem_t element);
