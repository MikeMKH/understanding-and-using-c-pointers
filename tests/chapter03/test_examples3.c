#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

Test(examples, hello_chapter_3) {
  char *pc = (char *)malloc(6 * sizeof(char));
  strcpy(pc, "Hello");
  cr_assert_str_eq(pc, "Hello");
  free(pc);
}
