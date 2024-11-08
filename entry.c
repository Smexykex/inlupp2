#include "entry.h"
#include "assert.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct ioopm_entry ioopm_entry_t;

ioopm_entry_t *ioopm_entry_create(elem_t key, elem_t value,
                                  ioopm_entry_t *next) {
  ioopm_entry_t *entry = calloc(1, sizeof(ioopm_entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;
  return entry;
}

void ioopm_entry_destroy(ioopm_entry_t *entry) { free(entry); }

void ioopm_entry_destroy_all(ioopm_entry_t *entry) {
  if (entry->next != NULL) {
    ioopm_entry_destroy_all(entry->next);
  }
  ioopm_entry_destroy(entry);
}

ioopm_entry_t *ioopm_find_previous_entry_for_key(ioopm_entry_t *entry,
                                                 elem_t key,
                                                 ioopm_eq_function *eq) {
  ioopm_entry_t *current = entry;
  while (current->next != NULL) {
    if (eq(current->next->key, key)) {
      return current;
    }
    current = current->next;
  }
  return entry;
}

size_t ioopm_entry_size(ioopm_entry_t *entry) {
  if (entry == NULL) {
    return 0;
  } else {
    return ioopm_entry_size(entry->next) + 1;
  }
}
