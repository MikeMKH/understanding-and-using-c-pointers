#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, basic) {
  int num = 67;
  int *pi = &num;
  cr_assert_eq(*pi, 67);
}