#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ask_question.h"
#include "linked_list.h"
#include "ui.h"

// TODO should make some functions static, though many are exposed for testing

// Asks question until a name that is in the hash table is entered.
static merch_t *ask_question_merch(merch_table_t *store, char *question)
{
  elem_t *lookup;
  char *name;

  do {
    name = ask_question_string(question);
    lookup = ioopm_hash_table_lookup(store, s_elem(name));
    free(name);
  } while (lookup == NULL);
  return lookup->p;
}

static char *ask_question_shelf(char *question)
{
  char *shelf;
  while (true) {
    shelf = ask_question_string(question);
    if (is_valid_shelf(shelf)) {
      break;
    }
    free(shelf);
  }
  // Lower case inputs are capitalised
  shelf[0] = toupper(shelf[0]);
  return shelf;
}

static cart_t *ask_question_cart(cart_table_t *cart_storage, char *question)
{
  size_t id;
  elem_t *lookup = NULL;

  while (lookup == NULL) {
    id = ask_question_size(question);
    lookup = ioopm_hash_table_lookup(cart_storage, i_elem(id));
  }

  return lookup->p;
}

void input_merch(merch_table_t *store)
{
  char *name = ask_question_string("\nName: ");
  char *description = ask_question_string("Description: ");
  size_t price = ask_question_size("Price: ");

  add_item_to_db(store, name, description, price);
  free(name);
  free(description);
}

void print_item(merch_t *merch)
{
  // TODO: does not print correctly if cents value less than 10
  printf("Name:  %10s | Desc:  %20s | Price: %zu.%2zu SEK\n", merch->name,
         merch->description, (merch->price) / 100, (merch->price) % 100);
}

void print_line() { puts("--------------------------------"); }

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp((*(merch_t *const *)p1)->name, (*(merch_t *const *)p2)->name);
}

bool stop_listing(size_t current_item)
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

void list_db(merch_table_t *store)
{
  size_t size = ioopm_hash_table_size(store);

  if (size == 0) {
    puts("Store is empty");
    return;
  }

  merch_t **items = (merch_t **)ioopm_hash_table_values(store);

  qsort(items, size, sizeof(elem_t *), cmpstringp);

  for (size_t current_item = 0; current_item < size; current_item++) {
    if (stop_listing(current_item)) {
      break;
    } else {
      print_item(items[current_item]);
    }
  }

  free(items);
}

void delete_merch(merch_table_t *store, cart_table_t *cart_storage)
{
  if (ioopm_hash_table_size(store) == 0) {
    puts("No items to delete!");
    return;
  }
  merch_t *merch = ask_question_merch(store, "\nInput merch name: ");

  char *confirmation =
      ask_question_string("Do you want delete?, type 'y' for yes ");

  if (tolower(confirmation[0]) == 'y') {
    remove_item_from_db(store, cart_storage, merch);
  }

  free(confirmation);
}

// TODO A better name might be edit item
void edit_db(merch_table_t *store)
{
  char *name_to_edit;
  elem_t *item;
  merch_t *merch_to_edit;

  do {
    name_to_edit = ask_question_string("\nInput merch name to edit: ");
    printf("%s\n", name_to_edit);
    item = ioopm_hash_table_lookup(store, s_elem(name_to_edit));

    free(name_to_edit);
  } while (item == NULL);

  merch_to_edit = (merch_t *)item->p;
  print_item(merch_to_edit);

  char *name;

  while (true) {
    name = ask_question_string("Input new name: ");
    if (ioopm_hash_table_lookup(store, s_elem(name)) == NULL) {
      break;
    }

    free(name);
  }

  char *description = ask_question_string("Input new description: ");
  size_t price = ask_question_size("Input new price: ");

  edit_merch(
      store, merch_to_edit->name,
      (merch_t){.name = name, .description = description, .price = price});

  free(name);
  free(description);
}

void show_stock(merch_table_t *store)
{
  merch_t *merch = ask_question_merch(store, "\nInput merch name: ");

  ioopm_list_iterator_t *iterator = ioopm_list_iterator(merch->locations);
  while (ioopm_iterator_has_next(iterator)) {
    location_t *location = ioopm_iterator_next(iterator).p;
    printf("%s: %3zu items\n", location->shelf, location->quantity);
  }
  free(iterator);
}

void replenish_stock(merch_table_t *store)
{
  merch_t *merch = ask_question_merch(store, "\nInput merch name: ");
  char *shelf;
  size_t quantity;

  while (true) {
    shelf = ask_question_shelf("\nInput shelf: ");
    quantity = ask_question_size("\nInput quantity to add: ");

    bool success = increase_stock(store, merch->name, shelf, quantity);
    free(shelf);
    if (success) {
      break;
    } else {
      printf("Shelf is taken by another item.");
    }
  };
}

cart_t *new_cart(cart_table_t *cart_storage, size_t id)
{
  cart_t *new_cart = create_cart(id);
  ioopm_hash_table_insert(cart_storage, i_elem(id), p_elem(new_cart));
  printf("New cart created with ID: %zu\n", id);
  return new_cart;
}

void remove_cart(cart_table_t *cart_storage)
{
  if (ioopm_hash_table_size(cart_storage) == 0) {
    printf("No carts in storage!\n");
    return;
  }

  cart_t *cart = ask_question_cart(cart_storage, "\nCart ID: ");

  char *confirmation = ask_question_string("Are you sure you want to remove? ");
  if (confirmation[0] == 'y' || confirmation[0] == 'Y') {
    destroy_cart(cart);
    ioopm_hash_table_remove(cart_storage, i_elem(cart->id));
  }

  free(confirmation);
}

void cart_add(merch_table_t *store, cart_table_t *cart_storage)
{
  if (ioopm_hash_table_size(cart_storage) == 0) {
    printf("No carts in storage!\n");
    return;
  }

  cart_t *cart = ask_question_cart(cart_storage, "\nCart ID: ");

  merch_t *merch = ask_question_merch(store, "Merch to add to cart: ");
  size_t quantity = ask_question_size("Amount of merch: ");

  bool success =
      add_to_cart(store, cart_storage, cart->id, merch->name, quantity);

  if (!success) {
    printf("Not enough in stock");
  }
}

void cart_remove(merch_table_t *store, cart_table_t *cart_storage)
{
  if (ioopm_hash_table_size(cart_storage) == 0) {
    printf("No carts in storage!\n");
    return;
  }

  cart_t *cart = ask_question_cart(cart_storage, "\nCart ID: ");

  merch_t *merch = ask_question_merch(store, "Merch to remove from cart: ");
  size_t quantity = ask_question_size("Amount of merch: ");

  bool success = remove_quantity_from_cart(cart, merch->name, quantity);

  if (!success) {
    printf("Not enough in cart");
  }
}

void get_cost(merch_table_t *store, cart_table_t *cart_storage)
{
  if (ioopm_hash_table_size(cart_storage) == 0) {
    printf("No carts in storage!\n");
    return;
  }

  cart_t *cart = ask_question_cart(cart_storage, "\nCart ID: ");

  size_t cost = calculate_cost(store, cart);

  printf("\nTotal cost for cart %zu: %zu.%2zu SEK", cart->id, (cost) / 100,
         (cost) % 100);
}

void checkout(merch_table_t *store, cart_table_t *cart_storage)
{
  if (ioopm_hash_table_size(cart_storage) == 0) {
    printf("No carts in storage!\n");
    return;
  }

  cart_t *cart = ask_question_cart(cart_storage, "\nCart ID: ");

  checkout_cart(store, cart);

  ioopm_hash_table_remove(cart_storage, u_elem(cart->id));
  destroy_cart(cart);
}

void confirm_quit()
{
  char *answer = ask_question_string("Confirm quit? (Y/n): ");
  if (tolower(answer[0] == 'y')) {
    exit(0);
  };
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
              "[Q]uit";
  printf("%s", str);
}

bool is_menu_option(char c)
{
  char *valid_inputs = "AaLlDdEeSsPpCcRr+-=OoQq";

  for (size_t i = 0; i < strlen(valid_inputs); i++) {
    if (c == valid_inputs[i]) {
      return true;
    }
  }

  return false;
}

char ask_question_menu()
{
  char *answer;

  do {
    print_menu();
    answer = ask_question_string("");
  } while (!(is_menu_option(answer[0])));

  return answer[0];
}

void event_loop()
{
  merch_table_t *store =
      ioopm_hash_table_create(string_sum_hash, str_eq_function);

  cart_table_t *cart_storage = ioopm_hash_table_create(NULL, NULL);
  char answer;
  size_t cart_id_next = 1;

  do {
    answer = ask_question_menu();

    print_line();

    answer = toupper(answer);

    switch (answer) {
      case 'A':
        input_merch(store);
        break;
      case 'L':
        list_db(store);
        break;
      case 'D':
        delete_merch(store, cart_storage);
        break;
      case 'E':
        edit_db(store);
        break;
      case 'S':
        show_stock(store);
        break;
      case 'P':
        replenish_stock(store);
        break;
      case 'C':
        new_cart(cart_storage, cart_id_next);
        cart_id_next++;
        break;
      case 'R':
        remove_cart(cart_storage);
        break;
      case '+':
        cart_add(store, cart_storage);
        break;
      case '-':
        cart_remove(store, cart_storage);
        break;
      case '=':
        get_cost(store, cart_storage);
        break;
      case 'O':
        checkout(store, cart_storage);
        break;
      case 'Q':
        confirm_quit();
        break;
      default:
        break;
    }
    print_line();
  } while (true);

  destroy_store(store);
}
