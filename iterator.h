/**
 * @file iterator.h
 * @author Mauritz Sjödin, Alvin Ljung Hallgren
 * @date 6 Oct 2024
 * @brief Iterator for a linked list.
 * Contains functions that make it easier to use an iterator.
 * Error handling is done with `assert`; we check if the pointer to the iterator is not `NULL`.
 * When needed. We also check if the iterator is on the last element in the underlying list.
 * 
 * Iterators are stored on the heap, so they must be freed with `ioopm_iterator_destroy`.
 * This does not free the underlying list.
 */

#pragma once
#include "common.h"

/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if there is at least one more element 
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
/// @return the next element
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter);

/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @return the current element
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);