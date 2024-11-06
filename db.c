#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "db.h"
#include "iterator.h"
#include "linked_list.h"
#include "utils.h"

extern char *strdup(const char *);

// Creates an empty locations list if NULL
merch_t *create_merch(char *namn, char *beskrivning, int pris,
                      ioopm_list_t *locations)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->namn = strdup(namn);
  merch->beskrivning = strdup(beskrivning);
  merch->pris = pris;
  merch->locations =
      locations != NULL ? locations : ioopm_linked_list_create(shelf_equals);
  return merch;
}

// does not edit locations
void edit_merch(ioopm_hash_table_t *store, char *name, merch_t new_value)
{
  merch_t *item =
      (merch_t *)ioopm_hash_table_lookup(store, str_elem(name))->any;
  assert(item != NULL);

  ioopm_hash_table_remove(store, str_elem(name));

  free(item->namn);
  free(item->beskrivning);

  item->namn = strdup(new_value.namn);
  item->beskrivning = strdup(new_value.beskrivning);
  item->pris = new_value.pris;
  item->locations = item->locations;

  ioopm_hash_table_insert(store, str_elem(item->namn), p_elem(item));
}

void destroy_merch(merch_t *merch)
{
  free(merch->namn);
  free(merch->beskrivning);

  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).any;
    free(location->shelf);
    free(location);
  }
  free(iterator);

  ioopm_linked_list_destroy(merch->locations);
  free(merch);
}

void destroy_store(ioopm_hash_table_t *store)
{
  size_t size = ioopm_hash_table_size(store);
  elem_t *items = ioopm_hash_table_values(store);

  for (size_t i = 0; i < size; i++) {
    destroy_merch(items[i].any);
  }

  ioopm_hash_table_destroy(store);
  free(items);
}

void add_item_to_db(ioopm_hash_table_t *store, char *name, char *description,
                    int price)
{
  merch_t *merch = create_merch(name, description, price, NULL);
  ioopm_hash_table_insert(store, str_elem(merch->namn), p_elem(merch));
}

bool is_shelf_taken(ioopm_hash_table_t *store, char *shelf)
{
  size_t size = ioopm_hash_table_size(store);
  elem_t *items = ioopm_hash_table_values(store);

  merch_t *item;
  location_t *location;

  for (size_t i = 0; i < size; i++) {
    item = items[i].any;

    ioopm_list_iterator_t *iterator = ioopm_list_iterator(item->locations);
    while (ioopm_iterator_has_next(iterator)) {
      location = ioopm_iterator_next(iterator).any;
      if (strcmp(location->shelf, shelf) == 0) {
        free(iterator);
        free(items);
        return true;
      }
    }
    free(iterator);
  }
  free(items);
  return false;
}

bool increase_stock(ioopm_hash_table_t *store, char *merch_name, char *shelf,
                    size_t add_quantity)
{
  if (!is_valid_shelf(shelf)) {
    return false;
  }

  elem_t *lookup = ioopm_hash_table_lookup(store, str_elem(merch_name));
  if (lookup == NULL) {
    return false;
  }
  merch_t *merch = lookup->any;
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).any;
    if (strcmp(location->shelf, shelf) == 0) {
      location->quantity += add_quantity;
      free(iterator);
      return true;
    }
  }
  free(iterator);

  if (is_shelf_taken(store, shelf)) {
    return false;
  };

  location_t *new_location = calloc(1, sizeof(location_t));
  new_location->shelf = strdup(shelf);
  new_location->quantity = add_quantity;

  ioopm_linked_list_append(merch->locations, p_elem(new_location));
  return true;
}

cart_t *create_cart(size_t id)
{
  cart_t *cart = calloc(1, sizeof(cart_t));

  cart->id = id;
  cart->items =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);
  return cart;
}

void destroy_cart(cart_t *cart)
{
  ioopm_hash_table_destroy(cart->items);
  free(cart);
}

struct name_sum {
  char *item_name;
  int sum;
};

void cart_quantity_sum(elem_t cart_id, elem_t *cart_elem, void *acc)
{
  cart_t *cart = cart_elem->any;
  if (cart == NULL) {
  }
  struct name_sum *ns = acc;
  elem_t *lookup =
      ioopm_hash_table_lookup(cart->items, str_elem(ns->item_name));

  if (lookup != NULL) {
    ns->sum += lookup->i;
  }
}

size_t get_total_stock(ioopm_hash_table_t *store, char *item_name)
{
  elem_t *lookup = ioopm_hash_table_lookup(store, str_elem(item_name));
  if (lookup == NULL) {
    return 0;
  }

  merch_t *item = lookup->any;
  size_t total_stock = 0;

  ioopm_list_iterator_t *iterator = ioopm_list_iterator(item->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).any;
    total_stock += location->quantity;
  }
  free(iterator);

  return total_stock;
}

void increase_cart_quantity(cart_t *cart, char *item_name, size_t quantity)
{
  elem_t *lookup = ioopm_hash_table_lookup(cart->items, str_elem(item_name));
  if (lookup != NULL) {
    lookup->u += quantity;
  } else {
    ioopm_hash_table_insert(cart->items, str_elem(item_name), u_elem(quantity));
  }
}

// carts: key: int id, value: cart_t cart
bool add_to_cart(ioopm_hash_table_t *store, ioopm_hash_table_t *carts,
                 size_t cart_id, char *item_name, size_t quantity)
{
  // get total stock in store
  size_t total_stock = get_total_stock(store, item_name);

  // get total already in carts
  struct name_sum ns = {.item_name = item_name, .sum = 0};
  ioopm_hash_table_apply_to_all(carts,
                                (ioopm_apply_function *)cart_quantity_sum, &ns);
  int total_in_carts = ns.sum;

  if (quantity + total_in_carts > total_stock) {
    return false;
  } else {
    elem_t *lookup = ioopm_hash_table_lookup(carts, u_elem(cart_id));
    if (lookup == NULL) {
      return false;
    }
    increase_cart_quantity(lookup->any, item_name, quantity);
    return true;
  }
}

// TODO
void remove_from_cart() { puts("Not yet implemented!"); }
