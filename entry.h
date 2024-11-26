#pragma once

#include "common.h"

/// Definition of ioopm_entry_t.
/// Consists of two elem_t:s. That are the (key, value) pair in the hash table.
/// next is a pointer the next ioopm_entry_t (if it exists), which creates a linked list
typedef struct ioopm_entry ioopm_entry_t;
struct ioopm_entry {
  elem_t key;
  elem_t value;
  ioopm_entry_t *next;
};

/// @brief Creates a new ioopm_entry_t and returns it
/// @param key The elem_t that contains the key part of the pair
/// @param value The elem_t that contains the value part of the pair
/// @return Pointer to the new ioopm_entry_t 
ioopm_entry_t *ioopm_entry_create(elem_t key, elem_t value,
                                  ioopm_entry_t *next);

/// @brief Destoys a ioopm_entry_t
/// @note Destroys up to 1 ioopm_entry_t, never more 
/// @param entry The entry to be destroyed
void ioopm_entry_destroy(ioopm_entry_t *entry);

/// @brief Destoys all ioopm_entry_t that are linked to the first 
/// @param entry The entry to be destroyed and all linked from it
void ioopm_entry_destroy_all(ioopm_entry_t *entry);

/// @brief Looks for a key in a linked list consisting of ioopm_entry_t.
/// it then returns the ioopm_entry_t one step before
/// @param entry The ioopm_entry_t where we start looking from
/// @param key The key of the ioopm_entry_t we are looking for
/// @param eq Equality function for comparing keys
/// @returns The ioopm_entry_t one step before the ioopm_entry_t we are looking for
/// Otherwise we retuns the ioopm_entry_t we started from 
ioopm_entry_t *ioopm_find_previous_entry_for_key(ioopm_entry_t *entry,
                                                 elem_t key,
                                                 ioopm_eq_function *eq);

/// @brief Calculates the size of the linked list of ioopm_entry_t
/// @param entry The ioopm_entry_t where we start calculating the size
/// @returns The size if the linked list of ioopm_entry_t
size_t ioopm_entry_size(ioopm_entry_t *entry);
