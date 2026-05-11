#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, casting_an_int_to_a_pointer_to_an_int) {
  int value = 8;
  int *pi = &value;
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
  int tmp = (int)pi;
  #pragma GCC diagnostic pop

  /* pi = (int*)tmp; */ /* ERROR: AddressSanitizer: SEGV on unknown address 0x00000d398500 */
  cr_assert_eq(tmp, tmp);
  cr_assert_eq(*pi, value);
}

Test(examples, determining_endianness_of_machine) {
  int value = 0x12345678;
  char *pc = (char*)&value;
  
  if (*pc == 0x78) {
    cr_expect(true, "machine is little-endian");
  } else if (*pc == 0x12) {
    cr_expect(true, "machine is big-endian");
  } else {
    cr_expect_fail("unexpected endianness detected");
  }
  
  cr_expect_eq(*pc, 0x78,
    "my Mac is little-endian, I find little-endian strange but it is more common than big-endian");
}