#include "hash_table.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct ioopm_hash_table {
  ioopm_hash_function *hash_function;
  ioopm_eq_function *key_equals;
  ioopm_entry_t buckets[NUM_BUCKETS]; // each bucket starts with a dummy,
  // ie first entry is found at buckets[i].next
};

static bool int_eq(elem_t a, elem_t b) { return a.i == b.i; }

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_function,
                                            ioopm_eq_function *key_equals)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->hash_function = hash_function;
  if (key_equals == NULL) {
    ht->key_equals = int_eq;
  } else {
    ht->key_equals = key_equals;
  }
  return ht;
}

static int find_bucket(ioopm_hash_table_t *ht, elem_t key)
{
  if (ht->hash_function == NULL) {
    // treat keys as integers
    return abs(key.i % NUM_BUCKETS);
  } else {
    return abs(ht->hash_function(key) % NUM_BUCKETS);
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < NUM_BUCKETS; i++) {
    if (ht->buckets[i].next != NULL)
      ioopm_entry_destroy_all(ht->buckets[i].next);
  }
  free(ht);
  return;
}

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert, cannot be NULL
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  const int bucket = find_bucket(ht, key);

  /// Search for an existing entry for a key
  ioopm_entry_t *prev = ioopm_find_previous_entry_for_key(&ht->buckets[bucket],
                                                          key, ht->key_equals);
  ioopm_entry_t *next = prev->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && ht->key_equals(next->key, key)) {
    next->value = value;
  } else {
    prev->next = ioopm_entry_create(key, value, next);
  }
}

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key
elem_t *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  const int bucket = find_bucket(ht, key);

  ioopm_entry_t *prev = ioopm_find_previous_entry_for_key(&ht->buckets[bucket],
                                                          key, ht->key_equals);
  ioopm_entry_t *next = prev->next;
  if (next != NULL && ht->key_equals(next->key, key)) {
    return &next->value;
  } else {
    return NULL;
  }
}

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return returns true if a value was removed
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  const int bucket = find_bucket(ht, key);

  ioopm_entry_t *prev = ioopm_find_previous_entry_for_key(&ht->buckets[bucket],
                                                          key, ht->key_equals);
  ioopm_entry_t *next = prev->next;

  if (next != NULL && ht->key_equals(next->key, key)) {
    prev->next = next->next;
    ioopm_entry_destroy(next);
    return true;
  } else {
    return false;
  }
}

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t total = 0;
  for (int i = 0; i < NUM_BUCKETS; i++) {
    total += ioopm_entry_size(ht->buckets[i].next);
  }
  return total;
}

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
//  Just slightly modified hash_table_destroy
void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < NUM_BUCKETS; i++) {
    if (ht->buckets[i].next != NULL) {
      ioopm_entry_destroy_all(ht->buckets[i].next);
      ht->buckets[i].next = NULL;
    }
  }
  return;
}

/// @brief return the keys for all entries in a hash map (in no particular
/// order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return an array of keys for hash table h (NULL if empty)
// Length of array needs to be known by caller
// TODO return linked_list instead of array
elem_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  // Awkward having to get size first alternative is to use stack memory but
  // then need to manage abitrarily long stack variables and allocating more
  // space as needed.
  const size_t size = ioopm_hash_table_size(ht);

  if (size == 0) {
    return NULL;
  }

  elem_t *keys = calloc(size, sizeof(elem_t));
  elem_t *k = keys;
  ioopm_entry_t *entry;

  for (int b = 0; b < NUM_BUCKETS; b++) {
    entry = ht->buckets[b].next;
    while (entry != NULL) {
      *k = entry->key;
      ++k;
      entry = entry->next;
    }
  }
  return keys;
}

/// @brief return the values for all entries in a hash map (in no particular
/// order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h (NULL if empty)
// TODO return linked_list instead of array
elem_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  // Same note as keys()
  const size_t size = ioopm_hash_table_size(ht);

  if (size == 0) {
    return NULL;
  }

  elem_t *values = calloc(size, sizeof(elem_t));
  elem_t *v = values;
  ioopm_entry_t *entry;

  for (int b = 0; b < NUM_BUCKETS; b++) {
    entry = ht->buckets[b].next;
    while (entry != NULL) {
      *v = entry->value;
      ++v;
      entry = entry->next;
    }
  }
  return values;
}

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  ioopm_entry_t *entry;

  for (int b = 0; b < NUM_BUCKETS; b++) {
    entry = ht->buckets[b].next;
    while (entry != NULL) {
      if (ht->key_equals(entry->key, key)) {
        return true;
      }
      entry = entry->next;
    }
  }
  return false;
}

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value,
                                ioopm_eq_function *f)
{
  ioopm_entry_t *entry;

  for (int b = 0; b < NUM_BUCKETS; b++) {
    entry = ht->buckets[b].next;
    while (entry != NULL) {
      if (f(entry->value, value)) {
        return true;
      }
      entry = entry->next;
    }
  }
  return false;
}

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate *pred,
                          void *arg)
{
  bool result = true;
  ioopm_entry_t *entry;

  for (int b = 0; b < NUM_BUCKETS; b++) {
    entry = ht->buckets[b].next;
    while (entry != NULL && result) {
      result = pred(entry->key, entry->value, arg);
      entry = entry->next;
    }
  }
  return result;
}

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate *pred,
                          void *arg)
{
  bool result = false;
  ioopm_entry_t *entry;

  for (int b = 0; b < NUM_BUCKETS; b++) {
    entry = ht->buckets[b].next;
    while (entry != NULL && !result) {
      result = pred(entry->key, entry->value, arg);
      entry = entry->next;
    }
  }
  return result;
}

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht,
                                   ioopm_apply_function *apply, void *arg)
{
  ioopm_entry_t *entry;

  for (int b = 0; b < NUM_BUCKETS; b++) {
    entry = ht->buckets[b].next;
    while (entry != NULL) {
      apply(entry->key, &entry->value, arg);
      entry = entry->next;
    }
  }
}
