/**
 * @file linked_list.h
 * @author Mauritz Sjödin, Alvin Ljung Hallgren
 * @date 6 Oct 2024
 * @brief Linked list and functions that operate on it.
 * Error handling is done with `assert`; we check if the pointer to the list is
 * not `NULL`. The functions `ioopm_linked_list_apply_to_all`,
 * `ioopm_linked_list_all` and `ioopm_linked_list_any` all requires the function
 * argument to be a defined valid function.
 *
 * Linked listss are stored on the heap, so they must be freed with
 * `ioopm_linked_list_destroy`.
 */

#pragma once

#include "common.h"
#include <stdbool.h>
#include <stdlib.h>

/// @brief Creates a new empty list
/// @param eq_func Function for determening equiality in the list
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function *eq_func);

/// @brief Tear down the linked list and return all its memory (but not the
/// memory of the elements)
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list
/// @param index the position in the list
/// @return true if the element was successfully removed otherwise false
bool ioopm_linked_list_remove(ioopm_list_t *list, int index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
elem_t ioopm_linked_list_get(ioopm_list_t *list, int index);

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param element the element sought
/// @return true if element is in the list, else false
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t ioopm_linked_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_linked_list_is_empty(ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
void ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all
/// internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate *prop,
                           void *extra);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all
/// internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate *prop,
                           void *extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all
/// internal calls of fun
void ioopm_linked_list_apply_to_all(ioopm_list_t *list,
                                    ioopm_apply_function *fun, void *extra);

/// @brief Get an iterator to a list
/// @param list the linked list
/// @return an pointer to an iterator for the list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);

/// @brief Get the index of an element in a list
/// @param list a list
/// @param elem an element in list
/// @return the index of elem, or -1 if elem is not in list
int ioopm_list_index_of(ioopm_list_t *list, elem_t elem);
