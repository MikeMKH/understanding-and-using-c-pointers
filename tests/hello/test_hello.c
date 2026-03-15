#include <criterion/criterion.h>
#include <stdlib.h>
#include "hello.h"

Test(hello, basic_name) {
  char *result = hello("Lily");
  cr_assert_str_eq(result, "Hello Lily!");
  free(result);
}

Test(hello, single_char_name) {
  char *result = hello("X");
  cr_assert_str_eq(result, "Hello X!");
  free(result);
}

Test(hello, empty_name) {
  char *result = hello("");
  cr_assert_str_eq(result, "Hello !");
  free(result);
}