#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, address_of_operator) {
  int num = 100;
  int *pi = &num;
  cr_assert_eq(*pi, 100);
  cr_assert_neq(pi, (int *)100, "congratulations, you have a pointer in lower memory!");
}

Test(examples, assignment_of_value) {
  int num = 100;
  int *pi = &num;
  *pi = 200;
  cr_assert_eq(num, 200);
}

Test(examples, pointer_to_void) {
  int num = 100;
  void *pv = &num;
  cr_assert_eq(*(int *)pv, 100);
  *(int *)pv = 200;
  cr_assert_eq(num, 200);
}

Test(examples, const_static_pointer_init_null) {
  /* text has just const int *pi; but gives a warning */
  const int *pi = NULL;
  cr_assert_eq(pi, NULL);
}
