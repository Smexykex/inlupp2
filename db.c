#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "db.h"
#include "hash_table.h"

extern char *strdup(const char *);

bool location_equals(elem_t a, elem_t b)
{
  location_t shelf_1 = *(location_t *)a.p;
  location_t shelf_2 = *(location_t *)b.p;

  return strcmp(shelf_1.shelf, shelf_2.shelf) == 0;
}

// Creates an empty locations list if NULL
merch_t *create_merch(char *name, char *description, size_t price,
                      location_list_t *locations)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->name = strdup(name);
  merch->description = strdup(description);
  merch->price = price;
  merch->locations =
      locations != NULL ? locations : ioopm_linked_list_create(location_equals);
  return merch;
}

void update_carts(cart_table_t *cart_storage, char *current_name,
                  char *new_name)
{
  elem_t *carts = ioopm_hash_table_values(cart_storage);

  for (size_t i = 0; i < ioopm_hash_table_size(cart_storage); i++) {
    cart_t *cart = carts[i].p;
    size_t quantity =
        ioopm_hash_table_lookup(cart->items, s_elem(current_name))->u;
    elem_t key = ioopm_hash_table_remove(cart->items, s_elem(current_name));
    free(key.s);
    ioopm_hash_table_insert(cart->items, s_elem(strdup(new_name)),
                            u_elem(quantity));
  }
  free(carts);
}

void edit_merch(merch_table_t *store, ioopm_hash_table_t *location_storage,
                char *name, merch_t new_value, cart_table_t *cart_storage)
{
  merch_t *item = (merch_t *)ioopm_hash_table_lookup(store, s_elem(name))->p;
  assert(item != NULL);

  ioopm_hash_table_remove(store, s_elem(name));

  update_carts(cart_storage, name, new_value.name);

  free(item->name);
  free(item->description);

  item->name = strdup(new_value.name);
  item->description = strdup(new_value.description);
  item->price = new_value.price;
  item->locations = item->locations;

  ioopm_hash_table_insert(store, s_elem(item->name), p_elem(item));

  // Edits location_storage. This might have wierd side effects if it's buggy.
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(item->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).p;
    ioopm_hash_table_insert(location_storage, s_elem(location->shelf),
                            s_elem(item->name));
  }
  free(iterator);
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

void destroy_store(merch_table_t *store)
{
  size_t size = ioopm_hash_table_size(store);
  elem_t *items = ioopm_hash_table_values(store);

  for (size_t i = 0; i < size; i++) {
    destroy_merch(items[i].p);
  }

  ioopm_hash_table_destroy(store);
  free(items);
}

void destroy_location_storage(ioopm_hash_table_t *location_storage)
{
  ioopm_hash_table_destroy(location_storage);
}

merch_t *add_item_to_db(merch_table_t *store, char *name, char *description,
                        size_t price)
{
  merch_t *merch = create_merch(name, description, price, NULL);
  ioopm_hash_table_insert(store, s_elem(merch->name), p_elem(merch));
  return merch;
}

void remove_from_location_storage(ioopm_hash_table_t *location_storage,
                                  merch_t *merch)
{
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).p;
    ioopm_hash_table_remove(location_storage, s_elem(location->shelf));
  }
  free(iterator);
}

static void remove_from_cart(elem_t id, elem_t *cart_e, void *item_name)
{
  cart_t *cart = cart_e->p;
  elem_t removed_key = ioopm_hash_table_remove(cart->items, s_elem(item_name));
  if (strcmp(removed_key.s, (char *) item_name)) {
    free(removed_key.s);
  }
}

void remove_item_from_db(merch_table_t *store,
                         ioopm_hash_table_t *location_storage,
                         cart_table_t *cart_storage, merch_t *merch)
{
  ioopm_hash_table_remove(store, s_elem(merch->name));

  remove_from_location_storage(location_storage, merch);

  if (cart_storage != NULL) {
    ioopm_hash_table_apply_to_all(cart_storage, remove_from_cart, merch->name);
  }
  destroy_merch(merch);
}

bool increase_stock(merch_table_t *store, ioopm_hash_table_t *location_storage,
                    char *item_name, char *shelf, size_t quantity)
{
  if (!is_valid_shelf(shelf)) {
    return false;
  }

  elem_t *lookup = ioopm_hash_table_lookup(store, s_elem(item_name));
  if (lookup == NULL) {
    return false;
  }

  elem_t *shelf_lookup =
      ioopm_hash_table_lookup(location_storage, s_elem(shelf));
  if (shelf_lookup != NULL && strcmp(shelf_lookup->s, item_name) != 0) {
    return false;
  }

  merch_t *merch = lookup->p;
  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).p;
    if (strcmp(location->shelf, shelf) == 0) {
      location->quantity += quantity;
      free(iterator);
      return true;
    }
  }
  free(iterator);

  location_t *new_location = calloc(1, sizeof(location_t));
  new_location->shelf = strdup(shelf);
  new_location->quantity = quantity;

  ioopm_linked_list_append(merch->locations, p_elem(new_location));
  ioopm_hash_table_insert(location_storage, s_elem(new_location->shelf),
                          s_elem(item_name));
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
  elem_t *keys = ioopm_hash_table_keys(cart->items);
  for (size_t i = 0; i < ioopm_hash_table_size(cart->items); i++) {
    free(keys[i].s);
  }
  free(keys);
  ioopm_hash_table_destroy(cart->items);
  free(cart);
}

void destroy_cart_storage(cart_table_t *cart_storage)
{
  elem_t *carts = ioopm_hash_table_values(cart_storage);

  for (size_t i = 0; i < ioopm_hash_table_size(cart_storage); i++) {
    cart_t *cart = carts[i].p;
    destroy_cart(cart);
  }

  free(carts);
  ioopm_hash_table_destroy(cart_storage);
}

struct name_sum {
  char *item_name;
  size_t sum;
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
    ioopm_hash_table_insert(cart->items, s_elem(strdup(item_name)),
                            u_elem(quantity));
  }
}

bool add_to_cart(merch_table_t *store, cart_table_t *cart_storage,
                 size_t cart_id, char *item_name, size_t quantity)
{
  size_t total_stock =
      get_total_stock(ioopm_hash_table_lookup(store, s_elem(item_name))->p);

  struct name_sum ns = {.item_name = item_name, .sum = 0};
  ioopm_hash_table_apply_to_all(cart_storage,
                                (ioopm_apply_function *)cart_quantity_sum, &ns);
  size_t total_in_carts = ns.sum;

  if (quantity + total_in_carts > total_stock) {
    return false;
  } else {
    elem_t *lookup = ioopm_hash_table_lookup(cart_storage, u_elem(cart_id));
    if (lookup == NULL) {
      return false;
    }
    increase_cart_quantity(lookup->p, item_name, quantity);
    return true;
  }
}

bool remove_quantity_from_cart(cart_t *cart, char *name, size_t quantity)
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

size_t calculate_cost(merch_table_t *store, cart_t *cart)
{
  elem_t *items = ioopm_hash_table_keys(cart->items);
  elem_t *quantities = ioopm_hash_table_values(cart->items);
  size_t size = ioopm_hash_table_size(cart->items);

  size_t sum = 0;

  for (size_t i = 0; i < size; i++) {
    merch_t *merch = ioopm_hash_table_lookup(store, items[i])->p;
    sum += merch->price * quantities[i].u;
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

void checkout_cart(merch_table_t *store, cart_t *cart)
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
