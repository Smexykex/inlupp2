#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "db.h"
#include "hash_table.h"
#include "iterator.h"
#include "linked_list.h"

extern char *strdup(const char *);

// Creates an empty locations list if NULL
merch_t *create_merch(char *name, char *description, int price,
                      ioopm_list_t *locations)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->name = strdup(name);
  merch->description = strdup(description);
  merch->price = price;
  merch->locations =
      locations != NULL ? locations : ioopm_linked_list_create(shelf_equals);
  return merch;
}

// does not edit locations
void edit_merch(ioopm_hash_table_t *store, char *name, merch_t new_value)
{
  merch_t *item = (merch_t *)ioopm_hash_table_lookup(store, s_elem(name))->p;
  assert(item != NULL);

  ioopm_hash_table_remove(store, s_elem(name));

  free(item->name);
  free(item->description);

  item->name = strdup(new_value.name);
  item->description = strdup(new_value.description);
  item->price = new_value.price;
  item->locations = item->locations;

  ioopm_hash_table_insert(store, s_elem(item->name), p_elem(item));
}

void destroy_location(location_t *location)
{
  free(location->shelf);
  free(location);
}

void destroy_merch(merch_t *merch)
{
  free(merch->name);
  free(merch->description);

  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).p;
    destroy_location(location);
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
    destroy_merch(items[i].p);
  }

  ioopm_hash_table_destroy(store);
  free(items);
}

merch_t *add_item_to_db(ioopm_hash_table_t *store, char *name,
                        char *description, int price)
{
  merch_t *merch = create_merch(name, description, price, NULL);
  ioopm_hash_table_insert(store, s_elem(merch->name), p_elem(merch));
  return merch;
}

bool is_shelf_taken(ioopm_hash_table_t *store, char *shelf)
{
  size_t size = ioopm_hash_table_size(store);
  elem_t *items = ioopm_hash_table_values(store);

  merch_t *item;
  location_t *location;

  for (size_t i = 0; i < size; i++) {
    item = items[i].p;

    ioopm_list_iterator_t *iterator = ioopm_list_iterator(item->locations);
    while (ioopm_iterator_has_next(iterator)) {
      location = ioopm_iterator_next(iterator).p;
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

  elem_t *lookup = ioopm_hash_table_lookup(store, s_elem(merch_name));
  if (lookup == NULL) {
    return false;
  }
  merch_t *merch = lookup->p;
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).p;
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
  cart->items = ioopm_hash_table_create(string_sum_hash, str_eq_function);
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
  cart_t *cart = cart_elem->p;
  if (cart == NULL) {
  }
  struct name_sum *ns = acc;
  elem_t *lookup = ioopm_hash_table_lookup(cart->items, s_elem(ns->item_name));

  if (lookup != NULL) {
    ns->sum += lookup->i;
  }
}

size_t get_total_stock(merch_t *merch)
{
  size_t total = 0;

  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).p;
    total += location->quantity;
  }
  free(iterator);

  return total;
}

void increase_cart_quantity(cart_t *cart, char *item_name, size_t quantity)
{
  elem_t *lookup = ioopm_hash_table_lookup(cart->items, s_elem(item_name));
  if (lookup != NULL) {
    lookup->u += quantity;
  } else {
    ioopm_hash_table_insert(cart->items, s_elem(item_name), u_elem(quantity));
  }
}

// carts: key: int id, value: cart_t cart
bool add_to_cart(ioopm_hash_table_t *store, ioopm_hash_table_t *carts,
                 size_t cart_id, char *item_name, size_t quantity)
{
  size_t total_stock =
      get_total_stock(ioopm_hash_table_lookup(store, s_elem(item_name))->p);

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
    increase_cart_quantity(lookup->p, item_name, quantity);
    return true;
  }
}

bool remove_from_cart(cart_t *cart, char *name, size_t quantity)
{
  elem_t *lookup = ioopm_hash_table_lookup(cart->items, s_elem(name));

  if (lookup == NULL) {
    return false;
  }

  size_t quantity_of_item = lookup->u;

  if (quantity > quantity_of_item) {
    return false;
  }

  lookup->u = quantity_of_item - quantity;
  return true;
}

size_t calculate_cost(ioopm_hash_table_t *store, cart_t *cart)
{
  char **items = &ioopm_hash_table_keys(cart->items)->s;
  size_t *quantities = &ioopm_hash_table_values(cart->items)->u;
  size_t size = ioopm_hash_table_size(cart->items);

  size_t sum = 0;

  for (size_t i = 0; i < size; i++) {
    merch_t *merch = ioopm_hash_table_lookup(store, s_elem(items[i]))->p;
    sum += merch->price * quantities[i];
  }
  free(items);
  free(quantities);
  return sum;
}

// Attempts to clear locations until quantity is removed
// Crashes if quantity is greater than total stock
static void decrease_stock(merch_t *merch, size_t quantity)
{
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);

  while (ioopm_iterator_has_next(iterator) && quantity > 0) {
    location_t *location = ioopm_iterator_current(iterator).p;

    if (location->quantity > quantity) {
      location->quantity -= quantity;
      free(iterator);
      return;
    } else {
      quantity -= location->quantity;
      destroy_location(location);
      ioopm_iterator_remove(iterator);
    }
  }
  free(iterator);

  assert(quantity == 0);
}

void checkout_cart(ioopm_hash_table_t *store, cart_t *cart)
{
  char **items = &ioopm_hash_table_keys(cart->items)->s;
  size_t *quantities = &ioopm_hash_table_values(cart->items)->u;
  size_t size = ioopm_hash_table_size(cart->items);

  for (size_t i = 0; i < size; i++) {
    merch_t *merch = ioopm_hash_table_lookup(store, s_elem(items[i]))->p;
    decrease_stock(merch, quantities[i]);
  }
  free(items);
  free(quantities);
}
