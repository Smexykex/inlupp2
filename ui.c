#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "utils.h"
#include "db.h"

void print_item(merch_t *vara)
{
  printf("Name:  %s\nDesc:  %s\nPrice: %d.%d SEK\nShelf: %s\n", 
    vara->namn, vara->beskrivning, (vara->pris) / 100, (vara->pris) % 100);
}

void print_line()
{
puts("--------------------------------");
}

// DISCONTINUED. TODO: FIX
/* void list_db(merch_t *items, int no_items)
{
  if (no_items == 0)
  {
    return;
  }
  
  print_line();

  for (int i = 0; i < no_items; i++)
  {
    merch_t current_item = items[i];
    printf("%2d: %s\n", i + 1, current_item.namn);
  }

  print_line();
} */

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

  for (int i = 0; i < strlen(valid_inputs); i++)
  {
    if (check_char == valid_inputs[i])
    {
      return true;
    }   
  }

  return false;    
}

char ask_question_menu()
{
  char answer[100];
  int string_size = 0;

  do 
  {
    print_menu();
    string_size = read_string(answer, 99);
  }
  while (!(string_size == 1 && is_menu_option(answer)));

  return answer[0];
}

void event_loop()
{
  ioopm_hash_table_t *merch_data_base = ioopm_hash_table_create(ioopm_str_eq_function, ioopm_int_eq_function, hash_function_void);
  char answer;
  void *cart_storage;
  int db_size = 0;
  int limit = 0;

  do
  {
    answer = ask_question_menu();
    answer = tolower(answer);

    switch (answer)
    {
    case 'a':
      if (db_size == limit)
      {
        printf("\nDatabase full!\n");
      }
      else
      {
        add_item_to_db(merch_data_base);
      }
      break;
    
    /* case 'l':
      if (db_size == 0)
      {
        printf("\nDatabase empty!\n\n");
      }
      else
      {
        list_db(db, db_size);
      }
      break; */

    case 'd':
      if (db_size == 0)
      {
        printf("\nDatabase empty!\n\n");
      }
      else
      {
        remove_item_from_db(merch_data_base);
      }
      break;

    case 'e':
      if (db_size == 0)
      {
        printf("\nDatabase empty!\n\n");
      }
      else
      {
        edit_db(merch_data_base);
      }
      break;
    
    case 's':
      show_stock(merch_data_base);
      break;
    
    case 'p':
      replenish_stock(merch_data_base);
      break;
    
    case 'c':
      create_cart(cart_storage);
      break;

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
    
    case 'o':
      checkout(cart_storage);
      break;

    default:
        break;
    }
  }
  while (answer != 'q'); 
}
