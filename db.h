#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "hash_table.h"
#include "linked_list.h"

// value: elem_t { .p = location }
typedef ioopm_list_t location_list_t;

// key: char *item_name, value: merch_t item
typedef ioopm_hash_table_t merch_table_t;

// key: char *item_name, value: size_t quantity
typedef ioopm_hash_table_t cart_items_table_t;

// key: size_t id, value: cart_t cart
typedef ioopm_hash_table_t cart_table_t;


typedef char *shelf_t;

// Definition of location_t. 
// Consists of the name of the shelf and how many are stored there
typedef struct location location_t;
struct location {
  shelf_t shelf;
  size_t quantity;
};

// Definition of merch_t.
// Consists of the name, description, price and locations of a merch. 
typedef struct merch merch_t;
struct merch {
  char *name;
  char *description;
  size_t price;
  location_list_t *locations;
};

// Definition of cart_t. 
// Consists of the id of the cart and what items the cart contains.
typedef struct cart cart_t;
struct cart {
  size_t id;
  cart_items_table_t *items;
};

/// @brief Destroys a location
/// @note Does not free the key and values, only the hash table
/// @param location pointer to the location to be freed 
void destroy_location(location_t *location);

/// @brief true if locations have the same shelf, ignores other values
/// @param a first location to check (as elem_t void *)
/// @param b second location to check (as elem_t void *)
bool location_equals(elem_t a, elem_t b);

/// @brief Creates a new merch, puts it on the heap and returns a pointer to it
/// @param store table to add to
/// @param name name of item
/// @param description description of item
/// @param price price of item
/// @return pointer to the created merch
merch_t *create_merch(char *name, char *description, size_t price,
                      location_list_t *locations);

/// @brief Edits a merch in the store by copying from a new merch
/// @note This will free the string and description strings
/// @param store table to edit from
/// @param location_storage location table to edit from
/// @param name name of item to edit
/// @param new_value new merch to copy data from
/// @param cart_storage cart table to edit from
void edit_merch(merch_table_t *store, ioopm_hash_table_t *location_storage,
                char *name, merch_t new_value, cart_table_t *cart_storage);

/// @brief Destroys a merch, which includes the name, description and locations
/// @param store Merch to destroy
void destroy_merch(merch_t *merch);

/// @brief Destroys a merch_table_t and all merch within
/// @param store table to destroy
void destroy_store(merch_table_t *store);

/// @brief Destroys a hash table with locations
/// @param store table to destroy
void destroy_location_storage(ioopm_hash_table_t *location_storage);

/// @brief creates a merch and inserts it into store
/// @param store table to add to
/// @param name name of item
/// @param description description of item
/// @param price price of item
/// @return pointer to the created merch
merch_t *add_item_to_db(merch_table_t *store, char *name, char *description,
                        size_t price);

/// @brief removes an item from the store and all carts that may contain it.
/// @param store table to remove from
/// @param cart_storage carts to remove from (can be NULL)
/// @param item_name name of item
void remove_item_from_db(merch_table_t *store,
                         ioopm_hash_table_t *location_storage,
                         cart_table_t *cart_storage, merch_t *merch);

/// @brief true if there exists an item in the store that has shelf in its
/// locations
/// @param store table containing items to check
/// @param shelf shelf label
bool is_shelf_taken(merch_table_t *store, char *shelf);

/// @brief if the shelf is not taken by another item, increases the quantity at
/// shelf of that item
/// @param store table containing all items to check
/// @param name name of item
/// @param shelf shelf to increase stock at
/// @param quantity quantity to add
/// @return true if successful
bool increase_stock(merch_table_t *store, ioopm_hash_table_t *location_storage,
                    char *item_name, char *shelf, size_t quantity);

/// @brief returns the sum of stock at all locations contained in an item
/// @param merch item to check
size_t get_total_stock(merch_t *merch);

/// @brief Creates a new cart
/// @param id The unqiue number that the new cart will have
cart_t *create_cart(size_t id);

/// @brief Destroys a cart
/// @param cart The cart to destroy
void destroy_cart(cart_t *cart);

/// @brief Destroys cart storage
/// @param cart_storage Where all carts are stored
void destroy_cart_storage(cart_table_t *cart_storage);

/// @brief increases the quantity of an item in a cart
/// @param cart cart to work on
/// @param item_name name of item to add
/// @param quantity quantity to add
void increase_cart_quantity(cart_t *cart, char *item_name, size_t quantity);

/// @brief increases the cart's quantity of an item after checking that the item
/// exists in store and that there is enough stock free relative to all carts
/// @param store table containing items to check
/// @param cart_storage table containing carts to check
/// @param cart_id id of cart to add to
/// @param item_name name of item to add
/// @param quantity quantity of item
/// @return true if successful
bool add_to_cart(merch_table_t *store, cart_table_t *cart_storage,
                 size_t cart_id, char *item_name, size_t quantity);

/// @brief removes a quantity of item from the cart, fails if item is not in
/// cart or quantity is greater than quantity in cart
/// @param cart cart to remove from
/// @param name name of item to remove
/// @param quantity quantity of item to remove
/// @return true if successful
bool remove_quantity_from_cart(cart_t *cart, char *name, size_t quantity);

/// @brief totals the cost of the cart by summing the item's prices by the
/// quantities in the cart
/// @param store table containing the items as cart does not store prices
/// @param cart cart to calculate cost for
size_t calculate_cost(merch_table_t *store, cart_t *cart);

/// @brief subtracts stock from each item that was in the cart
/// @param store store containing items to subtract from
/// @param cart cart containing items and quantities
void checkout_cart(merch_table_t *store, cart_t *cart);
