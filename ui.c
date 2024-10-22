#include "common.h"
#include "db.h"
#include "stdlib.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// TODO should make sme functions static

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

void list_db(ioopm_hash_table_t *store)
{
  size_t size = ioopm_hash_table_size(store);

  if (size == 0) {
    return;
  }

  merch_t **items = (merch_t **)ioopm_hash_table_values(store);

  qsort(items, size, sizeof(elem_t *), cmpstringp);

  for (int i = 0; i < size; i++) {
    if (i != 0 && i % 20 == 0) {
      char *answer;
      do {
        answer = ask_question_string("[L] for more, [N] to return");
        if (answer[0] == 'n' || answer[0] == 'N') {
          return;
        }
      } while (answer[0] != 'L');
    }
    print_item(items[i]);
  }

  free(items);
}

void print_menu()
{
  char *str = "[A]dd merch\n"
              "[L]ist merch\n"
              "[D]elete merch\n"
              "[E]dit merch\n"
              "[Q]uit\n";
  printf("%s", str);
}

bool is_menu_option(char *input_string)
{
  char *valid_inputs = "AaLlDdEeQq";
  char check_char = input_string[0];

  for (int i = 0; i < strlen(valid_inputs); i++) {
    if (check_char == valid_inputs[i]) {
      return true;
    }
  }

  return false;
}

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
  ioopm_hash_table_t *merch_data_base =
      ioopm_hash_table_create(hash_function_void, ioopm_str_eq_function);
  char answer;
  void *cart_storage = NULL;
  int db_size = 0;
  int limit = 0;

  do {
    answer = ask_question_menu();
    answer = tolower(answer);

    switch (answer) {
      case 'A':
      case 'a':
        if (db_size == limit) {
          printf("\nDatabase full!\n");
        } else {
          add_item_to_db(merch_data_base);
        }
        break;
      case 'L':
      case 'l':
        list_db(merch_data_base);
        break;
      case 'D':
      case 'd':
        if (db_size == 0) {
          printf("\nDatabase empty!\n\n");
        } else {
          remove_item_from_db(merch_data_base);
        }
        break;
      case 'E':
      case 'e':
        if (db_size == 0) {
          printf("\nDatabase empty!\n\n");
        } else {
          edit_db(merch_data_base);
        }
        break;
      case 'S':
      case 's':
        show_stock(merch_data_base);
        break;
      case 'P':
      case 'p':
        replenish_stock(merch_data_base);
        break;
      case 'C':
      case 'c':
        create_cart(cart_storage);
        break;
      case 'R':
      case 'r':
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
  } while (answer != 'q');
}

