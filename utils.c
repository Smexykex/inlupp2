#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"

bool is_number(char *str)
{
  if (strlen(str) == 0)
  {
    return false;
  }
  
  int index = 0;
  if (str[index] == '-') 
  {
    index = 1;
  }

  while (str[index] != '\0') 
  {
    if (str[index] < 48 || str[index] > 57) 
    {
      return false;
    }

    index++;
  }

  return true;
}

void clear_input_buffer() {
  int c;
  
  do 
  {
    c = getchar();
  }
  while (c != EOF && c != '\n');
}

int read_string(char *buf, int buffer_size) {
  int index = 0;
  int c;

  while (index < buffer_size) {
    c = getchar();

    if (c == EOF || c == '\n') {
      break;
    } else {
      buf[index] = c;
      index++;
    }
  }

  if (index == buffer_size) clear_input_buffer();

  buf[index] = '\0';
  return index;
}

void string_copy(char *source, char *dest, int offset)
{
  dest += offset;
  for (; *source != '\0'; ++dest, ++source)
  {
    *dest = *source;
  }
}

char *string_lower(char *str)
{
  char *str_copy = str;
  int current_char = 0;
  
  while (str_copy[current_char] != '\0')
  {
    str_copy[current_char] = tolower(str_copy[current_char]);
    current_char++;
  }

  return strdup(str_copy);
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func *check, convert_func *conv) {
  char answer[100];
  int string_size = 0;

  do 
  {
    printf("%s\n", question);
    string_size = read_string(answer, 99);
  }
  while (string_size == 0 || !check(answer));

  return conv(answer);
}

int ask_question_int(char *question) {
  answer_t ans = ask_question(question, is_number, (convert_func *) atoi);
  return ans.int_value;
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}