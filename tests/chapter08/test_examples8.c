#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, addition) {
  int a = 2;
  int b = 3;
  int sum = a + b;
  cr_assert_eq(sum, 5, "Expected %d but got %d", 5, sum);
}