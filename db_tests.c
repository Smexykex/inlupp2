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

void test_is_valid_shelf_false()
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
  char *shelf10 = "a-22";
  char *shelf11 = "a22.";

  CU_ASSERT_FALSE(is_valid_shelf(shelf1));
  CU_ASSERT_FALSE(is_valid_shelf(shelf2));
  CU_ASSERT_FALSE(is_valid_shelf(shelf3));
  CU_ASSERT_FALSE(is_valid_shelf(shelf4));
  CU_ASSERT_FALSE(is_valid_shelf(shelf5));
  CU_ASSERT_FALSE(is_valid_shelf(shelf6));
  CU_ASSERT_FALSE(is_valid_shelf(shelf7));
  CU_ASSERT_FALSE(is_valid_shelf(shelf8));
  CU_ASSERT_FALSE(is_valid_shelf(shelf9));
  CU_ASSERT_FALSE(is_valid_shelf(shelf10));
  CU_ASSERT_FALSE(is_valid_shelf(shelf11));
}

void test_is_valid_shelf_true()
{
  char *shelf1 = "a01";
  char *shelf2 = "A22";
  char *shelf3 = "b22";
  char *shelf4 = "z99";

  CU_ASSERT(is_valid_shelf(shelf1));
  CU_ASSERT(is_valid_shelf(shelf2));
  CU_ASSERT(is_valid_shelf(shelf3));
  CU_ASSERT(is_valid_shelf(shelf4));
}

void test_make_merch()
{
  char *name = "alvin";
  char *description = "test description";
  int price = 100;
  ioopm_list_t *merch_list = ioopm_linked_list_create(shelf_equals);

  merch_t *made_item = create_merch(name, description, price, merch_list);

  CU_ASSERT_STRING_EQUAL(made_item->name, "alvin");
  CU_ASSERT_STRING_EQUAL(made_item->description, "test description");
  CU_ASSERT_EQUAL(made_item->price, 100);
  CU_ASSERT_PTR_EQUAL(made_item->locations, merch_list);

  destroy_merch(made_item);
}

void test_add_merch()
{
  ioopm_hash_table_t *store =
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
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test", 100);

  merch_t *old_item = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;
  merch_t new_item = {.name = "max",
                      .description = "new desc",
                      .price = 200,
                      .locations = old_item->locations};

  edit_merch(store, "alvin", new_item);

  merch_t *edited_item = ioopm_hash_table_lookup(store, s_elem("max"))->p;
  assert(edited_item != NULL);

  CU_ASSERT_STRING_EQUAL(edited_item->name, "max");
  CU_ASSERT_STRING_EQUAL(edited_item->description, "new desc");
  CU_ASSERT_EQUAL(edited_item->price, 200);
  CU_ASSERT_PTR_EQUAL(edited_item->locations, old_item->locations);

  destroy_store(store);
}

void test_increase_stock()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  char *test_name = "alvin";

  add_item_to_db(store, test_name, "test description", 100);

  increase_stock(store, test_name, "A01", 1);

  merch_t *merch = ioopm_hash_table_lookup(store, s_elem(test_name))->p;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).p;

  CU_ASSERT_PTR_NOT_NULL(a01);
  CU_ASSERT_STRING_EQUAL(a01->shelf, "A01");
  CU_ASSERT_EQUAL(a01->quantity, 1);

  increase_stock(store, test_name, "A01", 1);

  CU_ASSERT_EQUAL(a01->quantity, 2);

  increase_stock(store, test_name, "B02", 1);

  location_t *b02 = ioopm_linked_list_get(merch->locations, 1).p;
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
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test description", 100);
  add_item_to_db(store, "max", "test description", 100);

  increase_stock(store, "alvin", "A01", 1);

  merch_t *merch = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).p;

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
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  add_item_to_db(store, "alvin", "test description", 100);

  increase_stock(store, "alvin", "A01", 1);
  increase_stock(store, "alvin", "A02", 15);

  merch_t *merch = ioopm_hash_table_lookup(store, s_elem("alvin"))->p;
  location_t *a01 = ioopm_linked_list_get(merch->locations, 0).p;
  location_t *a02 = ioopm_linked_list_get(merch->locations, 1).p;

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

void test_get_total_stock()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  char *item_name = "pen";
  add_item_to_db(store, item_name, "test", 100);
  size_t total_stock = get_total_stock(store, item_name);

  CU_ASSERT_EQUAL(total_stock, 0);

  increase_stock(store, item_name, "A1", 4);
  total_stock = get_total_stock(store, item_name);

  CU_ASSERT_EQUAL(total_stock, 4);

  char *new_item = "stick";
  add_item_to_db(store, new_item, "test", 100);
  increase_stock(store, item_name, "A1", 5);
  total_stock = get_total_stock(store, item_name);

  CU_ASSERT_EQUAL(total_stock, 9);

  destroy_store(store);
}

void test_add_to_cart()
{
  ioopm_hash_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  char *item_name = "pen";
  add_item_to_db(store, item_name, "test", 100);
  increase_stock(store, item_name, "A1", 4);

  cart_t *cart = create_cart(1);

  ioopm_hash_table_t *carts = ioopm_hash_table_create(NULL, NULL);

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
}

void test_remove_from_cart()
{
  cart_t *cart1 = create_cart(1);
  increase_cart_quantity(cart1, "phone", 5);
  remove_from_cart(cart1, "phone", 3);

  size_t quantity = ioopm_hash_table_lookup(cart1->items, s_elem("phone"))->u;

  CU_ASSERT_EQUAL(quantity, 2);

  remove_from_cart(cart1, "phone", 3);
  quantity = ioopm_hash_table_lookup(cart1->items, s_elem("phone"))->u;

  CU_ASSERT_EQUAL(quantity, 2);

  destroy_cart(cart1);
}

void test_calculate_cost()
{
  ioopm_hash_table_t *store =
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

int main()
{
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite my_test_suite =
      CU_add_suite("Data base test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((CU_add_test(my_test_suite, "is_valid_shelf false",
                   test_is_valid_shelf_false) == NULL) ||
      (CU_add_test(my_test_suite, "is_valid_shelf true",
                   test_is_valid_shelf_true) == NULL) ||
      (CU_add_test(my_test_suite, "make_merch", test_make_merch) == NULL) ||
      (CU_add_test(my_test_suite, "add_item_to_db", test_add_merch) == NULL) ||
      (CU_add_test(my_test_suite, "edit_merch", test_edit_merch) == NULL) ||
      (CU_add_test(my_test_suite, "Increase stock", test_increase_stock) ==
       NULL) ||
      (CU_add_test(my_test_suite, "Increase stock taken shelf",
                   test_increase_stock_taken_shelf) == NULL) ||
      (CU_add_test(my_test_suite, "Increase stock different quantities",
                   test_increase_stock_different_quantities) == NULL) ||
      (CU_add_test(my_test_suite, "Get total stock", test_get_total_stock) ==
       NULL) ||
      (CU_add_test(my_test_suite, "add_to_cart", test_add_to_cart) == NULL) ||
      (CU_add_test(my_test_suite, "calculate_cost", test_calculate_cost) ==
       NULL) ||
      0) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
