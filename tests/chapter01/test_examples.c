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

Test(examples, size_t_given_negative_value) {
  size_t x = -5;
  cr_assert_eq(x, (size_t)-5);
  
  char buffer[24];
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wformat"
  snprintf(buffer, sizeof(buffer), "%d", x);
  #pragma GCC diagnostic pop
  cr_assert_str_eq(buffer, "-5");
  
  snprintf(buffer, sizeof(buffer), "%zu", x);
  cr_assert_str_eq(buffer, "18446744073709551611");
}

Test(examples, pointer_addition_access_array_members) {
  int vector[] = {28, 41, 7};
  
  int *pi = vector;
  cr_assert_eq(*pi, 28);
  cr_assert_eq(pi, (int*)&vector,"pointer should point to vector + 0");
  
  pi += 1;
  cr_assert_eq(*pi, 41);
  cr_assert_eq(pi, (int*)((char*)&vector + sizeof(int)), "pointer should point to vector + 1");
  
  pi++;
  cr_assert_eq(*pi, 7);
  cr_assert_eq(pi, (int*)((char*)&vector + 2 * sizeof(int)), "pointer should point to vector + 2");
  
  pi += 1;
  cr_assert_eq(pi, (int*)((char*)&vector + 3 * sizeof(int)), "can continues past the end of the array");
  pi++;
  cr_assert_eq(pi, (int*)((char*)&vector + 4 * sizeof(int)), "... and so on");
}

Test(examples, pointer_subtraction) {
  int vector[] = {28, 41, 7};
  
  int *pi = vector + 2;
  cr_assert_eq(*pi, 7);
  
  pi -= 1;
  cr_assert_eq(*pi, 41);
  
  pi--;
  cr_assert_eq(*pi, 28);
}

Test(examples, pointer_comparison) {
  int vector[] = {28, 41, 7};
  
  int *p0 = vector;
  int *p1 = vector + 1;
  int *p2 = vector + 2;
  
  cr_assert_eq(p1 - p0, 1);
  cr_assert_eq(p2 - p0, 2);
  cr_assert_eq(p0 - p1, -1);
  
  cr_assert_eq(*p0 - *p1, -13, "can still deference pointers to get values");
  
  cr_assert(p0 + 1 == p1);
  cr_assert(p1 < p2);
  cr_assert(p2 > p1);
  cr_assert(p0 <= p0);
  cr_assert(p1 >= p0);
  cr_assert(p2 != p1);
}
