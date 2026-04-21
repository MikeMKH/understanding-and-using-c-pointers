#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

Test(examples, sizes) {
  cr_assert_eq(sizeof(char), 1);
  cr_assert_eq(sizeof('a'), 4, "character literals have type int, so sizeof('a') is the same as sizeof(int)");
}

Test(examples, string_literal_pool) {
  const char *s1 = "hello";
  cr_assert_str_eq(s1, "hello");
  const char *s2 = "hello";
  cr_expect_eq(s1, s2, "string literals with the same content may be stored at the same memory address");
}

/* my setup does not allow for modifying string literals */
/*
Test(examples, some_compilers_allow_for_modifying_string_literals) {
   char *s = "hello";
   s[0] = 'H';
   cr_expect_str_eq(s, "Hello");
 }
*/

Test(examples, string_initialization) {
  char local1[] = "Nine Inch Noize";
  cr_assert_str_eq(local1, "Nine Inch Noize");
  cr_assert_eq(sizeof(local1), 16);
  cr_assert_eq(strlen(local1), 15);
  
  char local2[] = {'N', 'i', 'n', 'e', ' ', 'I', 'n', 'c', 'h', ' ', 'N', 'o', 'i', 'z', 'e', '\0'};
  cr_assert_str_eq(local2, "Nine Inch Noize");
  cr_assert_eq(sizeof(local2), 16);
  
  char local3[16];
  strcpy(local3, "Nine Inch Noize");
  cr_assert_str_eq(local3, "Nine Inch Noize");
  cr_assert_eq(sizeof(local3), 16);
  
  char *heap = (char *)malloc(16);
  strcpy(heap, "Nine Inch Noize");
  cr_assert_str_eq(heap, "Nine Inch Noize");
  cr_assert_eq(sizeof(heap), sizeof(char *));
  
  static char static1[] = "Nine Inch Noize";
  cr_assert_str_eq(static1, "Nine Inch Noize");
  cr_assert_eq(sizeof(static1), 16);
  
  cr_expect(static1 != local1);
  cr_expect(heap    != local1);
  cr_expect(static1 != heap);
  
  /*
    Expected memory layout:
    - Lowest addresses: .rodata (literals, static data)
    - Middle addresses: Stack (local arrays grow downward)
    - Highest addresses: Heap (allocated memory)
  */
  cr_expect(heap   > local1);
  cr_expect(heap   > static1);
  cr_expect(static1 < local1);
  cr_expect(local1  < local2);
  cr_expect(local2  < local3);
  cr_expect(local3  < heap);
  
/*
  printf("actual memory addresses\n");
  printf("literal: %p\n", (void *)("Nine Inch Noize"));
  printf("local1:  %p\n", (void *)local1);
  printf("local2:  %p\n", (void *)local2);
  printf("local3:  %p\n", (void *)local3);
  printf("static1: %p\n", (void *)static1);
  printf("heap:   %p\n", (void *)heap);
*/
 
  free(heap);
}

Test(examples, compare_strings) {
  const char *s1 = "hi";
  const char *s2 = "hi";
  cr_assert_str_eq(s1, s2);
  cr_assert_eq(strcmp(s1, s2), 0);
  
  const char *s3 = "ai";
  cr_assert(strcmp(s1, s3) == 1);
  
  const char *s4 = "hz";
  cr_assert(strcmp(s1, s4) == -1);
}

Test(examples, string_copy) {
  char *buffer = malloc(30);
  
  char input[] = {'N', 'i', 'n', 'e', ' ', 'I', 'n', 'c', 'h', ' ', 'N', 'o', 'i', 'z', 'e', '\0'};
  sscanf(input, "%s", buffer);
  cr_assert_str_eq(buffer, "Nine");
  
  char *value = malloc(strlen(buffer) + 1);
  strcpy(value, buffer);
  cr_assert_str_eq(value, "Nine");
  
  free(buffer);
  free(value);
}

Test(examples, string_concatenation) {
  char *level = "Error";
  char *message = "File not found";
  char *buffer = malloc(strlen(level) + 2 + strlen(message) + 1);
  strcpy(buffer, level);
  strcat(buffer, ": ");
  strcat(buffer, message);
  cr_assert_str_eq(buffer, "Error: File not found");
  free(buffer);
}

size_t stringLength_byPointer(char *s) {
  size_t length = 0;
  while (*s++) length++;
  return length;
}

size_t stringLength_byArray(char s[]) {
  size_t length = 0;
  while (*s++) length++;
  return length;
}

size_t stringLength_byPointerToConstChar(const char *s) {
  size_t length = 0;
  while (*s++) length++;
  return length;
}

Test(example, stringLength_use) {
  char array[] = "Hello Lily!";
  char *p = (char *)malloc(strlen("Hello Lily!")+1);
  strcpy(p, "Hello Lily!");
  
  size_t expected = strlen("Hello Lily!");
  cr_assert_eq(stringLength_byPointer(array), expected);
  cr_assert_eq(stringLength_byPointer(p), expected);
  cr_assert_eq(stringLength_byArray(array), expected);
  cr_assert_eq(stringLength_byArray(p), expected);
  cr_assert_eq(stringLength_byPointerToConstChar(array), expected);
  cr_assert_eq(stringLength_byPointerToConstChar(p), expected);
}

char* format(char *buffer, size_t size, const char *name, unsigned int quantity, unsigned int weight) {
  char *fmt = "Item: %s Quantity: %u Weight: %u";
  if (buffer == NULL) {
    size_t len = strlen(fmt) + strlen(name) + (10 + 10) + 1;
    buffer = malloc(len);
  }
  
  snprintf(buffer, size, fmt, name, quantity, weight);
  return buffer;
}

Test(format, caller_allocates_output_string) {
  size_t size = 100;
  char *out = malloc(size);
  format(out, size, "Axle", 25, 45);
  cr_assert_str_eq(out, "Item: Axle Quantity: 25 Weight: 45");
  free(out);
}

Test(format, callee_allocates_output_string) {
  size_t size = 100;
  char *out = format(NULL, size, "Axle", 25, 45);
  cr_assert_str_eq(out, "Item: Axle Quantity: 25 Weight: 45");
  free(out);
}