#include "common.h"
#include "db.h"
#include "linked_list.h"
#include <CUnit/Basic.h>

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

void test_make_merch(void)
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

  ioopm_linked_list_destroy(merch_list);
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

// // TODO move to ui_tests
// void test_input_merch()
// {
//   merch_t made_item = input_merch();

//   CU_ASSERT_STRING_EQUAL(made_item.namn, "alvin");
//   CU_ASSERT_STRING_EQUAL(made_item.beskrivning, "test description");
//   CU_ASSERT_EQUAL(made_item.pris, 100);
//   CU_ASSERT_PTR_NOT_NULL(made_item.locations);

//   free(made_item.namn);
//   free(made_item.beskrivning);
//   ioopm_linked_list_destroy(made_item.locations);
// }

// // TODO move to ui_tests
// void test_add_item_to_empty_db()
// {
//   ioopm_hash_table_t *merch_data_base =
//       ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

//   input_merch(merch_data_base);

//   elem_t merch_key = {.str = "max"};
//   elem_t *added_item = ioopm_hash_table_lookup(merch_data_base, merch_key);

//   if (added_item == NULL) {
//     CU_ASSERT(false);
//   } else {
//     merch_t *added_merch = added_item->any;

//     CU_ASSERT_STRING_EQUAL(added_merch->namn, "max");
//     CU_ASSERT_STRING_EQUAL(added_merch->beskrivning, "test description");
//     CU_ASSERT_EQUAL(added_merch->pris, 100);
//     CU_ASSERT_PTR_NOT_NULL(added_merch->locations);
//   }

//   destroy_store(merch_data_base);
// }

// // TODO move to ui_tests
// void test_remove_item_from_db()
// {
//   ioopm_hash_table_t *merch_data_base =
//       ioopm_hash_table_create(ioopm_string_sum_hash, ioopm_str_eq_function);

//   add_item_to_db(merch_data_base);
//   add_item_to_db(merch_data_base);
//   elem_t *first_item =
//       ioopm_hash_table_lookup(merch_data_base, (elem_t){.str = "delete1"});
//   elem_t *second_item =
//       ioopm_hash_table_lookup(merch_data_base, (elem_t){.str = "delete2"});

//   remove_item_from_db(merch_data_base);

//   CU_ASSERT_EQUAL(ioopm_hash_table_size(merch_data_base), 1);
//   CU_ASSERT_FALSE(
//       ioopm_hash_table_has_key(merch_data_base, (elem_t){.str = "delete1"}));

//   destroy_store(merch_data_base);
// }

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
      // (CU_add_test(my_test_suite, "input_merch test", test_input_merch) ==
      //  NULL) ||
      // (CU_add_test(my_test_suite, "add_item_to_db on empty db test",
      //              test_add_item_to_empty_db) == NULL) ||
      //(CU_add_test(my_test_suite, "remove_item_from_db test",
      // test_remove_item_from_db) == NULL) ||
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
