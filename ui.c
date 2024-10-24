#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "db.h"
#include "hash_table.h"
#include "stdlib.h"
#include "ui.h"
#include "utils.h"

// TODO should make some functions static, though many are exposed for testing

void input_merch(ioopm_hash_table_t *store)
{
  char *name = ask_question_string("\nNamn: ");
  char *description = ask_question_string("Beskrivning: ");
  int price = ask_question_int("Pris: ");

  add_item_to_db(store, name, description, price);
  free(name);
  free(description);
}

void print_item(merch_t *vara)
{
  printf("Name:  %10s | Desc:  %20s | Price: %d.%d SEK\n", vara->namn,
         vara->beskrivning, (vara->pris) / 100, (vara->pris) % 100);
}

void print_line() { puts("--------------------------------"); }

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp((*(merch_t *const *)p1)->namn, (*(merch_t *const *)p2)->namn);
}

bool stop_listing(int current_item)
{
  char *answer;
  bool stop = false;

  if (current_item != 0 && current_item % 20 == 0) {
    do {
      answer =
          ask_question_string("Press [N] to return, any other button for more");
      stop = answer[0] == 'n' || answer[0] == 'N';

      free(answer);
    } while (!stop);
  }

  return stop;
}

void list_db(ioopm_hash_table_t *store)
{
  size_t size = ioopm_hash_table_size(store);

  if (size == 0) {
    puts("Store is empty");
    return;
  }

  merch_t **items = (merch_t **)ioopm_hash_table_values(store);

  qsort(items, size, sizeof(elem_t *), cmpstringp);

  for (int current_item = 0; current_item < size; current_item++) {
    if (stop_listing(current_item)) {
      break;
    } else {
      print_item(items[current_item]);
    }
  }

  free(items);
}

void remove_item_from_db(ioopm_hash_table_t *merch_data_base)
{
  char *merch_name;
  elem_t *retrived_value;

  do {
    merch_name = ask_question_string("\nInput merch name: ");
    retrived_value =
        ioopm_hash_table_lookup(merch_data_base, str_elem(merch_name));
  } while (retrived_value == NULL);

  char *confirmation =
      ask_question_string("Do you want delete?, type 'y' for yes ");

  if (tolower(confirmation[0]) == 'y') {
    merch_t *retrived_merch = retrived_value->any;

    ioopm_hash_table_remove(merch_data_base, str_elem(merch_name));
    destroy_merch(retrived_merch);
  }

  free(confirmation);
  free(merch_name);
}

void edit_db(ioopm_hash_table_t *store)
{
  char *name_to_edit;
  elem_t *item;
  merch_t *merch_to_edit;

  do {
    name_to_edit = ask_question_string("\nInput merch name to edit: ");
    printf("%s\n", name_to_edit);
    item = ioopm_hash_table_lookup(store, str_elem(name_to_edit));

    free(name_to_edit);
  } while (item == NULL);

  merch_to_edit = (merch_t *)item->any;
  print_item(merch_to_edit);

  char *name;

  while (true) {
    name = ask_question_string("Input new name: ");
    if (ioopm_hash_table_lookup(store, str_elem(name)) == NULL) {
      break;
    }

    free(name);
  }

  char *description = ask_question_string("Input new description: ");
  int price = ask_question_int("Input new price: ");

  edit_merch(
      store, merch_to_edit->namn,
      (merch_t){.namn = name, .beskrivning = description, .pris = price});

  free(name);
  free(description);
}

void print_menu()
{
  char *str = "[A]dd merch\n"
              "[L]ist merch\n"
              "[D]elete merch\n"
              "[E]dit merch\n"
              "[S]how stock\n"
              "Re[P]lenish stock\n"
              "[C]reate cart\n"
              "[R]emove cart\n"
              "[+] Add to cart\n"
              "[-] Remove from cart\n"
              "[=] Calculate cost\n"
              "Check[O]ut\n"
              "[Q]uit\n";
  printf("%s", str);
}

bool is_menu_option(char *input_string)
{
  char *valid_inputs = "AaLlDdEeSsPpCcRr+-=OoQq";
  char check_char = input_string[0];

  for (int i = 0; i < strlen(valid_inputs); i++) {
    if (check_char == valid_inputs[i]) {
      return true;
    }
  }

  return false;
}

// Could maybe rewrite using ask_question
char ask_question_menu()
{
  char answer[100];
  int string_size = 0;

  do {
    print_menu();
    string_size = read_string(answer, 99);
  } while (!(string_size == 1 && is_menu_option(answer)));

  return answer[0];
}

void event_loop()
{
  // Change hash function?
  ioopm_hash_table_t *merch_data_base =
      ioopm_hash_table_create(hash_function_void, ioopm_str_eq_function);
  char answer;
  void *cart_storage = NULL;
  int db_size = 0;
  int limit = 0;

  do {
    answer = ask_question_menu();

    // answer should always be upper case
    answer = toupper(answer);

    switch (answer) {
      case 'A':
        input_merch(merch_data_base);
        break;
      case 'L':
        list_db(merch_data_base);
        break;
      case 'D':
        remove_item_from_db(merch_data_base);
        break;
      case 'E':
        edit_db(merch_data_base);
        break;
      case 'S':
        show_stock(merch_data_base);
        break;
      case 'P':
        replenish_stock(merch_data_base);
        break;
      case 'C':
        create_cart(cart_storage);
        break;
      case 'R':
        remove_cart(cart_storage);
        break;
      case '+':
        add_to_cart(cart_storage);
        break;
      case '-':
        remove_from_cart(cart_storage);
        break;
      case '=':
        calculate_cost(cart_storage);
        break;
      case 'O':
        checkout(cart_storage);
        break;
      default:
        break;
    }
  } while (answer != 'Q');

  destroy_store(merch_data_base);
}
