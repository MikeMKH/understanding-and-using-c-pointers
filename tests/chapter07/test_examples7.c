#include <criterion/criterion.h>
#include <stdlib.h>

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