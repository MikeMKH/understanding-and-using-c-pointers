#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

Test(common_mistakes, forgetting_to_use_pointer_on_both_declarations) {
  int *pi1, pi2;
  
  // pointer pi1 can be assigned an address, but pi2 cannot
  pi2 = 42;
  pi1 = &pi2;
  cr_assert_eq(*pi1, pi2, "pi1 =pointer=> pi2");
}

Test(common_mistakes, using_macro_with_multiple_declarations_on_same_line) {
  #define PINT int*
  PINT pi1, pi2;
  
  // pointer pi1 can be assigned an address, but pi2 cannot
  pi2 = 42;
  pi1 = &pi2;
  cr_assert_eq(*pi1, pi2, "pi1 =pointer=> pi2");
  
  typedef int* PINT2;
  PINT2 pi3, pi4;
  int value = 42;
  // both pi3 and pi4 are pointers, so both can be assigned an address
  pi3 = &value;
  pi4 = pi3;
  cr_assert_eq(*pi3, *pi4);
}

Test(usage_issues, always_check_for_null_on_malloc) {
  char *str = malloc(500);
  if (str == NULL) {
    cr_expect_fail("malloc failed to allocate memory");
  }
  cr_assert_not_null(str, "malloc should return a non-null pointer");
  free(str);
}

Test(usage_issues, should_not_dereference_wild_pointer) {
  int *wild;
  /* ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x0001003fe6b3 bp 0x000304c656b0 sp 0x000304c654c0 T0) */
  /*wild = 42;*/  /* runtime error: store to null pointer of type 'int' */
  
  int num = 0;
  wild = &num;
  *wild = 42; 
  cr_assert_eq(*wild, 42, "wild pointer should be initialized before dereferencing");
}

Test(usage_issues, always_match_pointer_types) {
  int value = 2147483647;
  int *pi = &value;
  short *ps = (short*)pi;
  cr_assert_eq(*pi, value);
  cr_assert_eq(*pi, 2147483647);
  cr_assert_eq(*pi, 0x7FFFFFFF);
  cr_assert_neq(*ps, value);
  cr_assert_eq(*ps, -1);
  cr_assert_eq(*ps, (short)0xFFFF);
}

Test(deallocation_issues, double_free) {
  char *p = malloc(100);
  free(p);
  /* free(p); */  /* ERROR: AddressSanitizer: attempting double-free */
  cr_expect(true, "double free should be avoided");
}

Test(deallocation_issues, clear_sensitive_data) {
  // Demonstrating why you should clear sensitive data before freeing
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Waddress"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Waddress"
  
  char *password = malloc(20);
  strcpy(password, "supersecret");
  free(password);
  char *hacker = malloc(20);
  
  cr_assert_not_null(hacker, "malloc should return a pointer");
  cr_expect_not_null(memchr(hacker, 's', 20), "sensitive data may persist after free");
  
  #pragma clang diagnostic pop
  #pragma GCC diagnostic pop
  free(hacker);
}