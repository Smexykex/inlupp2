#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>

#include "db.h"
#include "hash_table.h"

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

void test_is_valid_shelf_false()
{
  CU_ASSERT_FALSE(is_valid_shelf("aa21"));
  CU_ASSERT_FALSE(is_valid_shelf("2a1"));
  CU_ASSERT_FALSE(is_valid_shelf("21A"));
  CU_ASSERT_FALSE(is_valid_shelf(" a12"));
  CU_ASSERT_FALSE(is_valid_shelf("aA12"));
  CU_ASSERT_FALSE(is_valid_shelf(".A12"));
  CU_ASSERT_FALSE(is_valid_shelf("A12 "));
  CU_ASSERT_FALSE(is_valid_shelf("A2 2"));
  CU_ASSERT_FALSE(is_valid_shelf("a 22"));
  CU_ASSERT_FALSE(is_valid_shelf("a-22"));
  CU_ASSERT_FALSE(is_valid_shelf("a22."));
}

void test_is_valid_shelf_true()
{
  CU_ASSERT(is_valid_shelf("a01"));
  CU_ASSERT(is_valid_shelf("A22"));
  CU_ASSERT(is_valid_shelf("b22"));
  CU_ASSERT(is_valid_shelf("z99"));
}

void test_make_merch()
{
  char *name = "alvin";
  char *description = "test description";
  size_t price = 100;
  ioopm_list_t *merch_list = ioopm_linked_list_create(location_equals);

  merch_t *made_item = create_merch(name, description, price, merch_list);

  CU_ASSERT_STRING_EQUAL(made_item->name, "alvin");
  CU_ASSERT_STRING_EQUAL(made_item->description, "test description");
  CU_ASSERT_EQUAL(made_item->price, 100);
  CU_ASSERT_PTR_EQUAL(made_item->locations, merch_list);

  destroy_merch(made_item);
}

void test_add_merch()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test", 100);

  merch_t *found_item = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;

  CU_ASSERT_STRING_EQUAL(found_item->name, "alvin");
  CU_ASSERT_STRING_EQUAL(found_item->description, "test");
  CU_ASSERT_EQUAL(found_item->price, 100);
  CU_ASSERT_PTR_NOT_NULL(found_item->locations);

  destroy_store(store);
}

void test_edit_merch()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  merch_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_table_t *cart_storage = ioopm_hash_table_create(NULL, NULL);

  add_item_to_db(store, "alvin", "test", 100);

  increase_stock(store, location_storage, "alvin", "A00", 1);

  merch_t *old_item = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;
  merch_t new_item = {.name = "max",
                      .description = "new desc",
                      .price = 200,
                      .locations = old_item->locations};

  edit_merch(store, location_storage, "alvin", new_item, cart_storage);

  merch_t *edited_item = ioopm_hash_table_lookup(store, s_elem("max"))->p;
  CU_ASSERT(edited_item != NULL);

  CU_ASSERT_STRING_EQUAL(edited_item->name, "max");
  CU_ASSERT_STRING_EQUAL(edited_item->description, "new desc");
  CU_ASSERT_EQUAL(edited_item->price, 200);
  CU_ASSERT_PTR_EQUAL(edited_item->locations, old_item->locations);

  destroy_store(store);
  destroy_location_storage(location_storage);
  destroy_cart_storage(cart_storage);
}

void test_increase_stock()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  char *test_name = "alvin";

  add_item_to_db(store, test_name, "test description", 100);

  increase_stock(store, location_storage, test_name, "A01", 1);

  merch_t *merch = ioopm_hash_table_lookup(store, s_elem(test_name))->p;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).p;

  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_STRING_EQUAL(a01->shelf, "A01");
  CU_ASSERT_EQUAL(a01->quantity, 1);

  increase_stock(store, location_storage, test_name, "A01", 1);

  CU_ASSERT_EQUAL(a01->quantity, 2);

  increase_stock(store, location_storage, test_name, "B02", 1);

  location_t *b02 = ioopm_linked_list_get(merch->locations, 1).p;
  CU_ASSERT_PTR_NOT_NULL(b02);
  CU_ASSERT_STRING_EQUAL(b02->shelf, "B02");
  CU_ASSERT_EQUAL(b02->quantity, 1);

  increase_stock(store, location_storage, test_name, "B02", 1);

  CU_ASSERT_EQUAL(a01->quantity, 2);

  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_increase_stock_taken_shelf()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test description", 100);
  add_item_to_db(store, "max", "test description", 100);

  increase_stock(store, location_storage, "alvin", "A01", 1);

  merch_t *merch = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).p;

  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_STRING_EQUAL(a01->shelf, "A01");
  CU_ASSERT_EQUAL(a01->quantity, 1);

  CU_ASSERT_FALSE(
      increase_stock(store, location_storage, "max", "invalid shelf", 1))
  CU_ASSERT_FALSE(
      increase_stock(store, location_storage, "item not in store", "A01", 1))
  CU_ASSERT_FALSE(increase_stock(store, location_storage, "max", "A01", 1))

  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_increase_stock_different_quantities()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test description", 100);

  increase_stock(store, location_storage, "alvin", "A01", 1);
  increase_stock(store, location_storage, "alvin", "A02", 15);

  merch_t *merch = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).p;
  location_t *a02 = ioopm_linked_list_get(merch->locations, 1).p;

  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_EQUAL(a01->quantity, 1);

  CU_ASSERT_PTR_NOT_NULL(a02);
  CU_ASSERT_EQUAL(a02->quantity, 15);

  increase_stock(store, location_storage, "alvin", "A01", 5);
  CU_ASSERT_EQUAL(a01->quantity, 6);

  // Negative numbers subtract, behaviour may be changed in future
  increase_stock(store, location_storage, "alvin", "A02", -5);
  CU_ASSERT_EQUAL(a02->quantity, 10);

  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_get_total_stock()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  char *item_name = "pen";
  merch_t *pen = add_item_to_db(store, item_name, "test", 100);
  size_t total_stock = get_total_stock(pen);

  CU_ASSERT_EQUAL(total_stock, 0);

  increase_stock(store, location_storage, item_name, "A1", 4);
  total_stock = get_total_stock(pen);

  CU_ASSERT_EQUAL(total_stock, 4);

  char *new_item = "stick";
  add_item_to_db(store, new_item, "test", 100);
  increase_stock(store, location_storage, item_name, "A1", 5);
  total_stock = get_total_stock(pen);

  CU_ASSERT_EQUAL(total_stock, 9);

  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_add_to_cart()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  char *item_name = "pen";
  add_item_to_db(store, item_name, "test", 100);
  increase_stock(store, location_storage, item_name, "A1", 4);

  cart_t *cart = create_cart(1);

  cart_table_t *carts = ioopm_hash_table_create(NULL, NULL);

  ioopm_hash_table_insert(carts, i_elem(1), p_elem(cart));

  add_to_cart(store, carts, 1, item_name, 2);

  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(cart->items, s_elem(item_name))->i,
                  2);

  add_to_cart(store, carts, 1, item_name, 5);

  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(cart->items, s_elem(item_name))->i,
                  2);

  add_to_cart(store, carts, 1, item_name, 1);

  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(cart->items, s_elem(item_name))->i,
                  3);

  destroy_cart(cart);
  ioopm_hash_table_destroy(carts);
  destroy_store(store);
  destroy_location_storage(location_storage);
}

void test_remove_from_cart()
{
  cart_t *cart1 = create_cart(1);
  increase_cart_quantity(cart1, "phone", 5);
  remove_quantity_from_cart(cart1, "phone", 3);

  CU_ASSERT_FALSE(remove_quantity_from_cart(cart1, "invalid name", 1));

  size_t quantity = ioopm_hash_table_lookup(cart1->items, s_elem("phone"))->u;

  CU_ASSERT_EQUAL(quantity, 2);

  remove_quantity_from_cart(cart1, "phone", 3);
  quantity = ioopm_hash_table_lookup(cart1->items, s_elem("phone"))->u;

  CU_ASSERT_EQUAL(quantity, 2);

  destroy_cart(cart1);
}

void test_calculate_cost()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_t *cart = create_cart(1);

  add_item_to_db(store, "item1", "desc", 100);
  add_item_to_db(store, "item2", "desc", 1);

  CU_ASSERT_EQUAL(calculate_cost(store, cart), 0);

  increase_cart_quantity(cart, "item1", 5);
  increase_cart_quantity(cart, "item2", 6);

  CU_ASSERT_EQUAL(calculate_cost(store, cart), 506);

  destroy_store(store);
  destroy_cart(cart);
}

void test_checkout_cart()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_t *cart = create_cart(1);

  merch_t *item1 = add_item_to_db(store, "item1", "desc", 1);
  merch_t *item2 = add_item_to_db(store, "item2", "desc", 1);

  increase_stock(store, location_storage, "item1", "A01", 5);
  increase_stock(store, location_storage, "item2", "B01", 2);
  increase_stock(store, location_storage, "item2", "B02", 5);

  increase_cart_quantity(cart, "item1", 5);
  increase_cart_quantity(cart, "item2", 4);

  checkout_cart(store, cart);

  CU_ASSERT_EQUAL(get_total_stock(item1), 0);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(item1->locations), 0);
  CU_ASSERT_EQUAL(get_total_stock(item2), 3);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(item2->locations), 1);

  destroy_store(store);
  destroy_location_storage(location_storage);
  destroy_cart(cart);
}

void test_remove_merch()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_table_t *cart_storage = ioopm_hash_table_create(NULL, NULL);

  cart_t *cart = create_cart(1);
  ioopm_hash_table_insert(cart_storage, u_elem(1), p_elem(cart));
  merch_t *merch1 = add_item_to_db(store, "test1", "test", 1);
  merch_t *merch2 = add_item_to_db(store, "test2", "test", 2);

  increase_stock(store, location_storage, "test1", "A01", 5);
  increase_stock(store, location_storage, "test2", "A02", 5);

  CU_ASSERT_FALSE(add_to_cart(store, cart_storage, 99, "test1", 1));

  add_to_cart(store, cart_storage, 1, "test1", 1);
  add_to_cart(store, cart_storage, 1, "test2", 1);

  remove_item_from_db(store, location_storage, cart_storage, merch1);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(store, s_elem("test1")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(cart->items, s_elem("test1")));
  CU_ASSERT(ioopm_hash_table_has_key(cart->items, s_elem("test2")));

  remove_item_from_db(store, location_storage, cart_storage, merch2);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(store, s_elem("test2")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(cart->items, s_elem("test2")));

  destroy_store(store);
  destroy_location_storage(location_storage);
  destroy_cart_storage(cart_storage);
}

void test_calculate_cost_after_edit()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  ioopm_hash_table_t *location_storage =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_table_t *cart_storage = ioopm_hash_table_create(NULL, NULL);

  add_item_to_db(store, "item", "test", 1);

  increase_stock(store, location_storage, "item", "A1", 5);

  cart_t *cart = create_cart(1);
  ioopm_hash_table_insert(cart_storage, u_elem(1), p_elem(cart));

  CU_ASSERT_FATAL(add_to_cart(store, cart_storage, 1, "item", 1));

  edit_merch(store, location_storage, "item",
             (merch_t){.name = "other", .description = "test", .price = 1},
             cart_storage);

  CU_ASSERT_EQUAL(calculate_cost(store, cart), 1);

  destroy_store(store);
  destroy_location_storage(location_storage);
  destroy_cart_storage(cart_storage);
}

int main()
{
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite db_tests =
      CU_add_suite("Data base test suite", init_suite, clean_suite);
  if (db_tests == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((CU_add_test(db_tests, "is_valid_shelf false",
                   test_is_valid_shelf_false) == NULL) ||
      (CU_add_test(db_tests, "is_valid_shelf true", test_is_valid_shelf_true) ==
       NULL) ||
      (CU_add_test(db_tests, "make_merch", test_make_merch) == NULL) ||
      (CU_add_test(db_tests, "add_item_to_db", test_add_merch) == NULL) ||
      (CU_add_test(db_tests, "edit_merch", test_edit_merch) == NULL) ||
      (CU_add_test(db_tests, "Increase stock", test_increase_stock) == NULL) ||
      (CU_add_test(db_tests, "Increase stock taken shelf",
                   test_increase_stock_taken_shelf) == NULL) ||
      (CU_add_test(db_tests, "Increase stock different quantities",
                   test_increase_stock_different_quantities) == NULL) ||
      (CU_add_test(db_tests, "Get total stock", test_get_total_stock) ==
       NULL) ||
      (CU_add_test(db_tests, "add_to_cart", test_add_to_cart) == NULL) ||
      (CU_add_test(db_tests, "remove_from_cart", test_remove_from_cart) ==
       NULL) ||
      (CU_add_test(db_tests, "calculate_cost", test_calculate_cost) == NULL) ||
      (CU_add_test(db_tests, "checkout_cart", test_checkout_cart) == NULL) ||
      (CU_add_test(db_tests, "remove merch", test_remove_merch) == NULL) ||
      (CU_add_test(db_tests, "Calculate cost after edit",
                   test_calculate_cost_after_edit) == NULL) ||
      0) {
    {
      CU_cleanup_registry();
      return CU_get_error();
    }
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
