#include "linked_list.h"
#include "iterator.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function *eq_func)
{
  ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
  list_entry_t *dummy = calloc(1, sizeof(list_entry_t));
  list->first = dummy;
  list->last = dummy;
  list->eq_func = eq_func;
  return list;
}

static void _linked_list_clear_helper(list_entry_t *entry)
{
  if (entry->next != NULL) {
    _linked_list_clear_helper(entry->next);
    free(entry->next);
  }
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  assert(list != NULL);

  if (list->first != NULL) {
    _linked_list_clear_helper(list->first);
    free(list->first);
  }
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  assert(list != NULL);

  ioopm_linked_list_clear(list);
  free(list);
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  assert(list != NULL);
  return list->size;
}

static elem_t _linked_list_get_helper(list_entry_t *entry, int index)
{
  assert(entry != NULL);

  return index == 0 ? entry->value
                    : _linked_list_get_helper(entry->next, index - 1);
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
  assert(list != NULL);
  return _linked_list_get_helper(list->first->next, index);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
  assert(list != NULL);

  list_entry_t *new_entry = malloc(sizeof(list_entry_t));
  new_entry->value = value;
  new_entry->next = NULL;

  list->last->next = new_entry;
  list->last = new_entry;
  list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
  assert(list != NULL);

  list_entry_t *new_entry = malloc(sizeof(list_entry_t));
  new_entry->value = value;
  new_entry->next = list->first->next;

  list->first->next = new_entry;

  if (list->last == list->first) {
    list->last = new_entry;
  }

  list->size++;
}

bool _linked_list_contains_helper(list_entry_t *entry, elem_t element,
                                  ioopm_eq_function *fun)
{
  return entry == NULL ? false
         : fun(entry->value, element)
             ? true
             : _linked_list_contains_helper(entry->next, element, fun);
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
  assert(list != NULL);
  return _linked_list_contains_helper(list->first->next, element,
                                      list->eq_func);
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  assert(list != NULL);
  return list->first->next == NULL;
}

bool ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
  assert(list != NULL);

  if (index == 0) {
    list_entry_t *to_remove = list->first->next;
    list->first->next = to_remove->next;
    list->size--;

    free(to_remove);
    return true;
  }

  size_t size = ioopm_linked_list_size(list);
  list_entry_t *entry = list->first->next;
  int current_index = 0;

  while (entry != NULL && current_index < size - 1) {
    if (current_index == index - 1) {
      list_entry_t *to_remove = entry->next;
      entry->next = to_remove->next;

      if (size == index + 1) {
        list->last = entry;
      }

      free(to_remove);
      list->size--;
      return true;
    }

    current_index++;
    entry = entry->next;
  }

  return false;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
  assert(list != NULL);

  if (index == 0) {
    ioopm_linked_list_prepend(list, value);
    return;
  } else if (index == ioopm_linked_list_size(list)) {
    ioopm_linked_list_append(list, value);
    return;
  }

  list_entry_t *entry = list->first->next;
  int current_index = 0;

  while (entry != NULL) {
    if (current_index == index - 1) {
      list_entry_t *new_elem = malloc(sizeof(list_entry_t));
      new_elem->value = value;
      new_elem->next = entry->next;

      entry->next = new_elem;
      list->size++;
      return;
    }

    current_index++;
    entry = entry->next;
  }
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate *prop,
                           void *extra)
{
  assert(list != NULL && prop != NULL);

  list_entry_t *curr_entry = list->first->next;

  while (curr_entry != NULL) {
    if (!prop(curr_entry->value, p_elem(extra), NULL)) {
      return false;
    }

    curr_entry = curr_entry->next;
  }

  return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate *prop,
                           void *extra)
{
  assert(list != NULL && prop != NULL);

  list_entry_t *curr_entry = list->first->next;

  while (curr_entry != NULL) {
    if (prop(curr_entry->value, p_elem(extra), NULL)) {
      return true;
    }

    curr_entry = curr_entry->next;
  }

  return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list,
                                    ioopm_apply_function *fun, void *extra)
{
  assert(list != NULL && fun != NULL);

  list_entry_t *current_entry = list->first->next;

  while (current_entry != NULL) {
    fun(current_entry->value, (elem_t *)extra, NULL);
    current_entry = current_entry->next;
  }
}

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  assert(list != NULL);

  ioopm_list_iterator_t *iterator = malloc(sizeof(ioopm_list_iterator_t));
  iterator->current = list->first, iterator->list = list;
  return iterator;
}

int ioopm_list_index_of(ioopm_list_t *list, elem_t elem)
{
  assert(list != NULL);

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  int index = 0;

  while (ioopm_iterator_has_next(iter)) {
    elem_t value = ioopm_iterator_next(iter);

    if (list->eq_func(value, elem)) {
      ioopm_iterator_destroy(iter);
      return index;
    }

    index++;
  }

  ioopm_iterator_destroy(iter);
  return -1;
}
