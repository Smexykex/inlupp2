#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stddef.h>

// Defines return type for all derived functions from ask_question
typedef union {
  int int_value;
  float float_value;
  char *string_value;
  size_t unsigned_value;
} answer_t;

// Function type that checks if a string is in a valid format 
typedef bool check_func(char *);

// Function type that converts a string to answer_t
typedef answer_t convert_func(char *);

/**
 * @brief Displays a question, then asks for an answer. Type-coverts this answer
 * to answer_t if it's in a correct format.
 * @param question String that is displayed in stdout.
 * @param check Function that checks format of answer.
 * @param conv Function that converts answer to specifed type.
 * @returns Converted answer 
 */
answer_t ask_question(char *question, check_func *check, convert_func *conv);

/**
 * @brief Displays a question, then asks for an answer. Type-coverts this answer
 * to size_t if it's in a correct format.
 * @param question String that is displayed in stdout.
 * @returns Converted answer
 */
size_t ask_question_size(char *question);

/**
 * @brief Displays a question, then asks for an answer. Type-coverts this answer
 * to int if it's in a correct format.
 * @param question String that is displayed in stdout.
 * @returns Converted answer
 */
int ask_question_int(char *question);

/**
 * @brief Displays a question, then asks for an answer. Type-coverts this answer
 * to string if it's in a correct format.
 * @param question String that is displayed in stdout.
 * @returns Converted answer
 */
char *ask_question_string(char *question);

#endif
