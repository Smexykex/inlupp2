#include <CUnit/Basic.h>

#include "common.h"
#include "db.h"
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

void edit_item_test()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  input_merch(store);
  input_merch(store);

  edit_db(store);

  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(store, str_elem("alvin")));

  merch_t *item =
      (merch_t *)ioopm_hash_table_lookup(store, str_elem("max"))->any;

  CU_ASSERT_PTR_NOT_NULL(item);
  CU_ASSERT_STRING_EQUAL(item->namn, "max");
  CU_ASSERT_STRING_EQUAL(item->beskrivning, "different test description");
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

  if (CU_add_test(my_test_suite, "Edit item", edit_item_test) == NULL || 0) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
