#include "common.h"
#include "db.h"
#include "hash_table.h"
#include "linked_list.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <assert.h>
#include <time.h>

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

/* void test_is_item_false()
{
  char *shelf1 = "aa21";
  char *shelf2 = "2a1";
  char *shelf3 = "21A";
  char *shelf4 = " a12";
  char *shelf5 = "aA12";
  char *shelf6 = ".A12";
  char *shelf7 = "A12 ";
  char *shelf8 = "A2 2";
  char *shelf9 = "a 22";
  char *shelf9 = "a-22";
  char *shelf10 = "a22.";

  CU_ASSERT_FALSE(is_item(shelf1));
  CU_ASSERT_FALSE(is_item(shelf2));
  CU_ASSERT_FALSE(is_item(shelf3));
  CU_ASSERT_FALSE(is_item(shelf4));
  CU_ASSERT_FALSE(is_item(shelf5));
  CU_ASSERT_FALSE(is_item(shelf6));
  CU_ASSERT_FALSE(is_item(shelf7));
  CU_ASSERT_FALSE(is_item(shelf8));
  CU_ASSERT_FALSE(is_item(shelf9));
  CU_ASSERT_FALSE(is_item(shelf10));
}

void test_is_item_true()
{
  char *shelf1 = "a01";
  char *shelf2 = "A22";
  char *shelf3 = "b22";
  char *shelf4 = "z99";

  CU_ASSERT(is_item(shelf1));
  CU_ASSERT(is_item(shelf2));
  CU_ASSERT(is_item(shelf3));
  CU_ASSERT(is_item(shelf4));
} */

void test_make_merch()
{
  char *name = "alvin";
  char *description = "test description";
  int price = 100;
  ioopm_list_t *merch_list = ioopm_linked_list_create(shelf_equals);

  merch_t *made_item = create_merch(name, description, price, merch_list);

  CU_ASSERT_STRING_EQUAL(made_item->namn, "alvin");
  CU_ASSERT_STRING_EQUAL(made_item->beskrivning, "test description");
  CU_ASSERT_EQUAL(made_item->pris, 100);
  CU_ASSERT_PTR_EQUAL(made_item->locations, merch_list);

  destroy_merch(made_item);
}

void test_add_merch()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  add_item_to_db(store, "alvin", "test", 100);

  merch_t *found_item = ioopm_hash_table_lookup(store, str_elem("alvin"))->any;

  CU_ASSERT_STRING_EQUAL(found_item->namn, "alvin");
  CU_ASSERT_STRING_EQUAL(found_item->beskrivning, "test");
  CU_ASSERT_EQUAL(found_item->pris, 100);
  CU_ASSERT_PTR_NOT_NULL(found_item->locations);

  destroy_store(store);
}

void test_edit_merch()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  add_item_to_db(store, "alvin", "test", 100);

  merch_t *old_item = ioopm_hash_table_lookup(store, str_elem("alvin"))->any;
  merch_t new_item = {.namn = "max",
                      .beskrivning = "new desc",
                      .pris = 200,
                      .locations = old_item->locations};

  edit_merch(store, "alvin", new_item);

  merch_t *edited_item = ioopm_hash_table_lookup(store, str_elem("max"))->any;
  assert(edited_item != NULL);

  CU_ASSERT_STRING_EQUAL(edited_item->namn, "max");
  CU_ASSERT_STRING_EQUAL(edited_item->beskrivning, "new desc");
  CU_ASSERT_EQUAL(edited_item->pris, 200);
  CU_ASSERT_PTR_EQUAL(edited_item->locations, old_item->locations);

  destroy_store(store);
}

void test_increase_stock()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  char *test_name = "alvin";

  add_item_to_db(store, test_name, "test description", 100);

  increase_stock(store, test_name, "A01", 1);

  merch_t *merch = ioopm_hash_table_lookup(store, str_elem(test_name))->any;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).any;

  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_STRING_EQUAL(a01->shelf, "A01");
  CU_ASSERT_EQUAL(a01->quantity, 1);

  increase_stock(store, test_name, "A01", 1);

  CU_ASSERT_EQUAL(a01->quantity, 2);

  increase_stock(store, test_name, "B02", 1);

  location_t *b02 = ioopm_linked_list_get(merch->locations, 1).any;
  CU_ASSERT_PTR_NOT_NULL(b02);
  CU_ASSERT_STRING_EQUAL(b02->shelf, "B02");
  CU_ASSERT_EQUAL(b02->quantity, 1);

  increase_stock(store, test_name, "B02", 1);

  CU_ASSERT_EQUAL(a01->quantity, 2);

  destroy_store(store);
}

void test_increase_stock_taken_shelf()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  add_item_to_db(store, "alvin", "test description", 100);
  add_item_to_db(store, "max", "test description", 100);

  increase_stock(store, "alvin", "A01", 1);

  merch_t *merch = ioopm_hash_table_lookup(store, str_elem("alvin"))->any;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).any;

  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_STRING_EQUAL(a01->shelf, "A01");
  CU_ASSERT_EQUAL(a01->quantity, 1);

  CU_ASSERT_FALSE(increase_stock(store, "max", "invalid shelf", 1))
  CU_ASSERT_FALSE(increase_stock(store, "item not in store", "A01", 1))
  CU_ASSERT_FALSE(increase_stock(store, "max", "A01", 1))

  destroy_store(store);
}

void test_increase_stock_different_quantities()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

  add_item_to_db(store, "alvin", "test description", 100);

  increase_stock(store, "alvin", "A01", 1);
  increase_stock(store, "alvin", "A02", 15);

  merch_t *merch = ioopm_hash_table_lookup(store, str_elem("alvin"))->any;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).any;
  location_t *a02 = ioopm_linked_list_get(merch->locations, 1).any;

  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_EQUAL(a01->quantity, 1);

  CU_ASSERT_PTR_NOT_NULL(a02);
  CU_ASSERT_EQUAL(a02->quantity, 15);

  increase_stock(store, "alvin", "A01", 5);
  CU_ASSERT_EQUAL(a01->quantity, 6);

  // Negative numbers subtract, behaviour may be changed in future
  increase_stock(store, "alvin", "A02", -5);
  CU_ASSERT_EQUAL(a02->quantity, 10);

  destroy_store(store);
}

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite =
      CU_add_suite("Data base test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
    // If the test suite could not be added, tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if ((CU_add_test(my_test_suite, "make_merch test", test_make_merch) ==
       NULL) ||
      (CU_add_test(my_test_suite, "add_item_to_db test", test_add_merch) ==
       NULL) ||
      (CU_add_test(my_test_suite, "edit_merch test", test_edit_merch) ==
       NULL) ||
      (CU_add_test(my_test_suite, "Increase stock", test_increase_stock) ==
       NULL) ||
      0) {
    // If adding any of the tests fails, we tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
