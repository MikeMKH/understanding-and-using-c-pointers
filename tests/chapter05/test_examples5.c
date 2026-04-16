#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, init) {
  cr_assert(true, "this test just ensures the test framework is working");
}