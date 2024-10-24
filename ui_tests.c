#include <CUnit/Basic.h>

#include "common.h"
#include "db.h"
#include "hash_table.h"
#include "ui.h"

int init_suite(void)
{
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void)
{
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

void test_input_merch()
{
  ioopm_hash_table_t *merch_data_base =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  input_merch(merch_data_base);

  elem_t merch_key = {.str = "max"};
  elem_t *added_item = ioopm_hash_table_lookup(merch_data_base, merch_key);

  if (added_item == NULL) {
    CU_ASSERT(false);
  } else {
    merch_t *added_merch = added_item->any;

    CU_ASSERT_STRING_EQUAL(added_merch->namn, "max");
    CU_ASSERT_STRING_EQUAL(added_merch->beskrivning, "test description");
    CU_ASSERT_EQUAL(added_merch->pris, 100);
    CU_ASSERT_PTR_NOT_NULL(added_merch->locations);
  }

  destroy_store(merch_data_base);
}

void test_remove_item()
{
  ioopm_hash_table_t *merch_data_base =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  add_item_to_db(merch_data_base, "alvin", "test", 100);
  add_item_to_db(merch_data_base, "max", "test", 100);

  remove_item_from_db(merch_data_base);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(merch_data_base), 1);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(merch_data_base, str_elem("alvin")));

  destroy_store(merch_data_base);
}

void test_edit_item()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  // input_merch(store);
  // input_merch(store);
  add_item_to_db(store, "alvin", "test1", 100);
  add_item_to_db(store, "elias", "test2", 100);

  edit_db(store);

  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(store, str_elem("alvin")));

  merch_t *item =
      (merch_t *)ioopm_hash_table_lookup(store, str_elem("max"))->any;

  CU_ASSERT_PTR_NOT_NULL(item);
  CU_ASSERT_STRING_EQUAL(item->namn, "max");
  CU_ASSERT_STRING_EQUAL(item->beskrivning, "test3");
  CU_ASSERT_EQUAL(item->pris, 200);

  destroy_store(store);
}

int main()
{
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite my_test_suite =
      CU_add_suite("UI test suite", init_suite, clean_suite);

  if (my_test_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(my_test_suite, "Add item", test_input_merch) == NULL ||
      CU_add_test(my_test_suite, "Remove item", test_remove_item) == NULL ||
      CU_add_test(my_test_suite, "Edit item", test_edit_item) == NULL || 0) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
