#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

int assert_adder(int a, int b) {
  int result = a + b;
  cr_assert(&a < &b, "first argument should be at a lower memory address than second argument");
  /*
    ARM64 Local variables grow downward but don't necessarily have predictable ordering relative 
    to register-passed arguments like they do on x86-64, so we can't reliably assert the ordering 
    of local variables relative to arguments on ARM64
  */
  cr_assert(&result > &a, "local variable should be at a higher memory address than arguments");
  return result;
}

Test(examples, organization_of_program_stack) {
  int accumulator = 0;
  for(int i = 0; i < 10; i += 2) {
    int sum = assert_adder(i, i+1);
    cr_assert(&sum > &accumulator, "block variable should be at a lower memory address than local variable");
    accumulator += sum;
  }
  cr_assert_eq(accumulator, 45, "the sum of [0, 9] is 45");
}
