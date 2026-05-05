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