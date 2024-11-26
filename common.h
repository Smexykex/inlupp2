#pragma once

#include <stdbool.h>
#include <stdlib.h>

// Defines a union that are the values in the hash table
typedef union elem elem_t;
union elem {
  int i;
  size_t u;
  bool b;
  float f;
  void *p;
  char *s;
};

// Macros for easier usage of hash tables
#define s_elem(x)                                                              \
  (elem_t) { .s = x }
#define p_elem(x)                                                              \
  (elem_t) { .p = x }
#define i_elem(x)                                                              \
  (elem_t) { .i = x }
#define u_elem(x)                                                              \
  (elem_t) { .u = x }

// Function types for the hash table
typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);


/// @brief Checks if two varibles of type elem_t contains an integer 
/// and that those intergers are equal
/// @param a First elem_t
/// @param a Second elem_t
/// @returns True if the integers in a and b are equal, false otherwise
bool int_eq_function(elem_t a, elem_t b);


/// @brief Checks if two varibles of type elem_t contains a float
/// and that those floats are equal
/// @param a First elem_t
/// @param a Second elem_t
/// @returns True if the floats in a and b are equal, false otherwise
bool float_eq_function(elem_t a, elem_t b);

/// @brief Checks if two varibles of type elem_t contains a boolean
/// and that those booleans are equal
/// @param a First elem_t
/// @param a Second elem_t
/// @returns True if the booleans in a and b are equal, false otherwise
bool bool_eq_function(elem_t a, elem_t b);

/// @brief Checks if two varibles of type elem_t contains a string
/// and that those strings are equal
/// @param a First elem_t
/// @param a Second elem_t
/// @returns True if the strings in a and b are equal, false otherwise
bool str_eq_function(elem_t a, elem_t b);

// Hash function for hash tables with strings as values
int string_sum_hash(const elem_t key1);

// Checks for strings that are valid shelfs
bool is_valid_shelf(char *shelf);
