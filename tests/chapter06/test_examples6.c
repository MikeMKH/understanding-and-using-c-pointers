#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, pointer_to_string_literal) {
  const char *s = "Hello World!";
  cr_assert_str_eq(s, "Hello World!");
  cr_assert_eq(sizeof(s), sizeof(char *));
  cr_assert_eq(strlen(s), 12);
}