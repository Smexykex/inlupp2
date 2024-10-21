#pragma once

#include "common.h"

typedef struct ioopm_entry ioopm_entry_t;

struct ioopm_entry {
  elem_t key;          // holds the key
  elem_t value;        // holds the value
  ioopm_entry_t *next; // points to the next entry (possibly NULL)
};

ioopm_entry_t *ioopm_entry_create(elem_t key, elem_t value,
                                  ioopm_entry_t *next);

void ioopm_entry_destroy(ioopm_entry_t *entry);

void ioopm_entry_destroy_all(ioopm_entry_t *entry);

ioopm_entry_t *ioopm_find_previous_entry_for_key(ioopm_entry_t *entry,
                                                 elem_t key,
                                                 ioopm_eq_function *eq);

int ioopm_entry_size(ioopm_entry_t *entry);
