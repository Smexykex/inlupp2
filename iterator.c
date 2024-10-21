#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "common.h"
#include "iterator.h"

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
  assert(iter != NULL);
  iter->current = iter->list->first;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  assert(iter != NULL);
  return iter->current->next != NULL; 
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
  assert(iter != NULL && ioopm_iterator_has_next(iter));
  return iter->current->next->value; 
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
  assert(iter != NULL && ioopm_iterator_has_next(iter));

  list_entry_t *to_remove = iter->current->next;
  elem_t result = to_remove->value;
  if (to_remove == iter->list->first->next)
  {
    iter->list->first->next = to_remove->next;
  }
  else 
  {
    //iter->current->next = to_remove->next;
    if (to_remove == iter->list->last)
    {
      iter->list->last = iter->current;
    }
  }
  
  iter->current->next = to_remove->next; 
  iter->list->size--;

  free(to_remove); 

  return result;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  assert(iter != NULL && ioopm_iterator_has_next(iter));

  iter->current = iter->current->next;
  return iter->current->value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  assert(iter != NULL);
  free(iter);
}