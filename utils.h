#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

/**
 * @brief Displays a question, then asks for an answer. Type-coverts this answer if it's in a correct format.
 * @param question String that is displayed in stdout.
 * @param check Function that checks format of answer.
 * @param conv Function that converts answer to specifed type.
 */
answer_t ask_question(char *question, check_func *check, convert_func *conv);

int ask_question_int(char *question);
char *ask_question_string(char *question);

extern char *strdup(const char *);
int read_string(char *buf, int buf_siz);

/**
 * @brief Copies one string into another
 * @param soucre Source string to copy
 * @param dest Destination string to copy into
 * @param offset What index in destination string to start copying into. 0 means no displacement 
 */
void string_copy(char *source, char *dest, int offset);

/**
 * @brief Returns a copy of a string with upper case letters replaced with lower case.
 * @param str String that is copied
 * @return Altered string
 */
char *string_lower(char *str);


bool is_number(char *str);

#endif 