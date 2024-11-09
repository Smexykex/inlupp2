#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

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
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  input_merch(store);

  elem_t merch_key = {.s = "max"};
  elem_t *added_item = ioopm_hash_table_lookup(store, merch_key);

  if (added_item == NULL) {
    CU_ASSERT(false);
  } else {
    merch_t *added_merch = added_item->p;

    CU_ASSERT_STRING_EQUAL(added_merch->name, "max");
    CU_ASSERT_STRING_EQUAL(added_merch->description, "test description");
    CU_ASSERT_EQUAL(added_merch->price, 100);
    CU_ASSERT_PTR_NOT_NULL(added_merch->locations);
  }

  destroy_store(store);
}

void test_remove_item()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test", 100);
  add_item_to_db(store, "max", "test", 100);

  delete_merch(store, location_storage, NULL);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(store), 1);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(store, s_elem("alvin")));

  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_edit_item()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test1", 100);
  add_item_to_db(store, "elias", "test2", 100);

  edit_db(store, location_storage);

  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(store, s_elem("alvin")));

  merch_t *item = (merch_t *)ioopm_hash_table_lookup(store, s_elem("max"))->p;

  CU_ASSERT_PTR_NOT_NULL(item);
  CU_ASSERT_STRING_EQUAL(item->name, "max");
  CU_ASSERT_STRING_EQUAL(item->description, "test3");
  CU_ASSERT_EQUAL(item->price, 200);

  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_replenish_stock()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test description", 100);

  // Add location A01
  replenish_stock(store, location_storage);
  merch_t *merch = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;

  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).p;
  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_STRING_EQUAL(a01->shelf, "A01");
  CU_ASSERT_EQUAL(a01->quantity, 1);

  // Input location A01 again, should increase quantity
  replenish_stock(store, location_storage);

  CU_ASSERT_EQUAL(a01->quantity, 2);

  // Not existing location B02, should add new location
  replenish_stock(store, location_storage);

  location_t *b02 = ioopm_linked_list_get(merch->locations, 1).p;
  CU_ASSERT_PTR_NOT_NULL(b02);
  CU_ASSERT_STRING_EQUAL(b02->shelf, "B02");
  CU_ASSERT_EQUAL(b02->quantity, 1);

  // Input location B02 again, should increase quantity
  replenish_stock(store, location_storage);

  CU_ASSERT_EQUAL(a01->quantity, 2);

  // Finally show stock, need to verify correct in output
  show_stock(store);

  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_cart_add()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_table_t *cart_storage = ioopm_hash_table_create(NULL, NULL);

  add_item_to_db(store, "phone", "test", 100);
  increase_stock(store, location_storage, "phone", "A0", 5);
  cart_t *cart1 = new_cart(cart_storage, 1);

  cart_add(store, cart_storage);
  cart_add(store, cart_storage);

  size_t quantity = ioopm_hash_table_lookup(cart1->items, s_elem("phone"))->u;

  CU_ASSERT_EQUAL(quantity, 5);

  cart_t *cart2 = new_cart(cart_storage, 2);
  cart_add(store, cart_storage);

  elem_t *lookup = ioopm_hash_table_lookup(cart2->items, s_elem("phone"));

  CU_ASSERT_PTR_NULL(lookup);

  destroy_store(store);
  destroy_location_storage(location_storage);
  ioopm_hash_table_destroy(cart_storage);
  destroy_cart(cart1);
  destroy_cart(cart2);
}

void test_cart_remove()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_table_t *cart_storage = ioopm_hash_table_create(NULL, NULL);

  add_item_to_db(store, "phone", "test", 100);
  increase_stock(store, location_storage, "phone", "A0", 5);
  cart_t *cart1 = new_cart(cart_storage, 1);

  add_to_cart(store, cart_storage, 1, "phone", 5);

  cart_remove(store, cart_storage);
  size_t quantity = ioopm_hash_table_lookup(cart1->items, s_elem("phone"))->u;

  CU_ASSERT_EQUAL(quantity, 2);

  cart_remove(store, cart_storage);
  quantity = ioopm_hash_table_lookup(cart1->items, s_elem("phone"))->u;

  CU_ASSERT_EQUAL(quantity, 2);

  destroy_store(store);
  destroy_location_storage(location_storage);
  ioopm_hash_table_destroy(cart_storage);
  destroy_cart(cart1);
}

int main()
{
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite ui_tests = CU_add_suite("UI test suite", init_suite, clean_suite);

  if (ui_tests == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((CU_add_test(ui_tests, "Add item", test_input_merch) == NULL) ||
      (CU_add_test(ui_tests, "Remove item", test_remove_item) == NULL) ||
      (CU_add_test(ui_tests, "Edit item", test_edit_item) == NULL) ||
      (CU_add_test(ui_tests, "Replenish stock", test_replenish_stock) ==
       NULL) ||
      (CU_add_test(ui_tests, "Add to cart", test_cart_add) == NULL) ||
      (CU_add_test(ui_tests, "Remove from cart", test_cart_remove) == NULL) ||
      0) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
