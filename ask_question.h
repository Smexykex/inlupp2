#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stddef.h>

typedef union {
  int int_value;
  float float_value;
  char *string_value;
  size_t unsigned_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

/**
 * @brief Displays a question, then asks for an answer. Type-coverts this answer
 * if it's in a correct format.
 * @param question String that is displayed in stdout.
 * @param check Function that checks format of answer.
 * @param conv Function that converts answer to specifed type.
 */
answer_t ask_question(char *question, check_func *check, convert_func *conv);

size_t ask_question_size(char *question);
int ask_question_int(char *question);
char *ask_question_string(char *question);

#endif
