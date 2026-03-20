#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, basic_malloc_use) {
  int *pi = (int *)malloc(sizeof(int));
  *pi = 5;
  cr_assert_eq(*pi, 5);
  free(pi);
}

Test(examples, heap_manager_data_structure) {
  char *pc = (char *)malloc(6 * sizeof(char));
  cr_assert_not_null(pc+7, "malloc should allocate extra memory for heap manager data structure");
  free(pc);
}

Test(example, need_safe_pointer_to_avoid_memory_leaks) {
  char *pc = (char *)malloc(strlen("Mike") + 1);
  char *free_me = pc;
  strcpy(pc, "Mike");
  cr_assert_str_eq(pc, "Mike");
  while(*pc != '\0') pc++;
  cr_assert_eq(*pc, '\0');
  /* if we did not have a pointer to the original memory location, we would not be able to free it now */
  free(free_me);
}
