#pragma once

#include "db.h"

/// @brief Adds a merch to the merch database by asking for a name, a
/// description and a price.
/// @param store Hashtable containting all of the merch
void input_merch(merch_table_t *store);

/// @brief Prints a merch to stdout
/// @param merch Pointer to the merch to be printed
void print_item(merch_t *merch);

/// @brief Prints all merch in the merch database. 20 merch are listed at a time
/// and requires user input to print more
/// @param store Hashtable containting all of the merch
void list_db(merch_table_t *store);

void delete_merch(merch_table_t *store, cart_table_t *cart_storage);

/// @brief Edits a merch in the merch database by asking for the name of a
/// merch. It then asks for a new name, description and price.
/// @param store Hashtable containting all of the merch
void edit_db(merch_table_t *store);

/// @brief Prints the location and quantity of a merch. It asks for the name of
/// merch in the merch database.
/// @param store Hashtable containting all of the merch
void show_stock(merch_table_t *store);

/// Adds a given amount of a merch to a given location to the merch database. It
/// asks for a merch name in the merch database, a shelf that contains that
/// specific merch or a new shelf and a quantity of new merch.
/// @note Cannot add to a shelf with a different merch.
/// @param store Hashtable containting all of the merch
void replenish_stock(merch_table_t *store);

/// @brief Creates a new cart, adds it to the cart database and returns it.
/// @param cart_storage Hashtable containing all of the carts.
/// @param id Unique number to identify the cart.
/// @return Pointer to the created cart.
cart_t *new_cart(cart_table_t *cart_storage, size_t id);

/// @brief Adds merch the a cart. It asks for the id of a cart, the name of the
/// merch to add to the cart and the quantity of merch.
/// @note Can only add to cart if the total stock of the merch in the merch
/// database is bigger than the quantity in all carts.
/// @param store Hashtable containting all of the merch
/// @param cart_storage Hashtable containing all of the carts.
void cart_add(merch_table_t *store, cart_table_t *cart_storage);

/// @brief Removes merch from a cart. It asks for the id of a cart, the name of
/// the merch to remove from the cart and the quantity of merch.
/// @note Cannot remove more of a merch than is already in the cart.
/// @param store Hashtable containting all of the merch
/// @param cart_storage Hashtable containing all of the carts.
void cart_remove(merch_table_t *store, cart_table_t *cart_storage);

/// @brief Starts the program. All required data structures are contained in the
/// function and are freed from memory when it is done.
void event_loop();
