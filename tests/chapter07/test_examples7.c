#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, addition) {
  int a = 5;
  int b = 10;
  int sum = a + b;
  
  cr_assert_eq(sum, 15);
}