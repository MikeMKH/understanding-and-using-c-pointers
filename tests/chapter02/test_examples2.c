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

Test(examples, need_safe_pointer_to_avoid_memory_leaks) {
  char *pc = (char *)malloc(strlen("Mike") + 1);
  char *free_me = pc;
  strcpy(pc, "Mike");
  cr_assert_str_eq(pc, "Mike");
  while(*pc != '\0') pc++;
  cr_assert_eq(*pc, '\0');
  /* if we did not have a pointer to the original memory location, we would not be able to free it now */
  free(free_me);
}

Test(examples, malloc_does_not_zero_out_memory) {
  int *pi = (int *)malloc(sizeof(int));
  cr_assert_not_null(pi, "malloc should not return NULL");
  cr_expect(*pi != 0, "congratulations, you got zeroed out memory"); /* malloc does not zeroed memory */
  *pi = 5;
  cr_assert_eq(*pi, 5);
  free(pi);
}

Test(examples, malloc_wants_total_size) {
  int NUMBER_OF_DOUBLES = 3;
  double *pd = (double *)malloc(NUMBER_OF_DOUBLES * sizeof(double));
  cr_assert_not_null(pd, "malloc should not return NULL");
  pd[0] = 3.14;
  pd[1] = 2.718;
  pd[2] = 1.618;
  cr_assert_eq(pd[0], 3.14);
  cr_assert_eq(pd[1], 2.718);
  cr_assert_eq(pd[2], 1.618);
  free(pd);
}

Test(examples, calloc_does_zero_out_memory) {
  int *pi = (int *)calloc(1, sizeof(int));
  cr_assert_not_null(pi, "calloc should not return NULL");
  cr_assert_eq(*pi, 0, "calloc should zero out memory");
  *pi += 5;
  cr_assert_eq(*pi, 5);
  free(pi);
}

Test(examples, calloc_wants_number_of_elements_and_size) {
  int NUMBER_OF_DOUBLES = 3;
  double *pd = (double *)calloc(NUMBER_OF_DOUBLES, sizeof(double));
  cr_assert_not_null(pd, "calloc should not return NULL");
  cr_assert_eq(pd[0], 0.0);
  cr_assert_eq(pd[1], 0.0);
  cr_assert_eq(pd[2], 0.0);
  pd[0] = 3.14;
  pd[1] = 2.718;
  pd[2] = 1.618;
  cr_assert_eq(pd[0], 3.14);
  cr_assert_eq(pd[1], 2.718);
  cr_assert_eq(pd[2], 1.618);
  free(pd);
}

Test(examples, realloc_can_resize_memory) {
  int *pi = (int *)malloc(2 * sizeof(int));
  cr_assert_not_null(pi, "malloc should not return NULL");
  pi[0] = 10;
  pi[1] = 20;
  cr_assert_eq(pi[0], 10);
  cr_assert_eq(pi[1], 20);
  
  pi = (int *)realloc(pi, 3 * sizeof(int));
  cr_assert_not_null(pi, "realloc should not return NULL");
  pi[2] = 30;
  cr_assert_eq(pi[0], 10);
  cr_assert_eq(pi[1], 20);
  cr_assert_eq(pi[2], 30);
  
  free(pi);
}

Test(examples, realloc_can_shrink_memory) {
  char *s1 = (char *)malloc(16);
  strcpy(s1, "123456789ABCDEF");
  cr_assert_str_eq(s1, "123456789ABCDEF");
  
  char *s2 = (char *)realloc(s1, 8);
  /* realloc may or may not move the memory, so the content may or may not be preserved */
  /* cr_expect_str_eq(s2, "123456789ABCDEF"); */ /* Shadow bytes around the buggy address */
  s2[7] = '\0';
  cr_assert_str_eq(s2, "1234567");
  free(s2);
}

Test(examples, assigning_null_to_free_pointer) {
  char *s = (char *)malloc(16);
  strcpy(s, "Hello, world!");
  cr_assert_str_eq(s, "Hello, world!");
  free(s);
  /* s is a dangling pointer */
  /* cr_assert_str_eq(s, "Hello, world!"); */ /* ERROR: AddressSanitizer: heap-use-after-free */
  s = NULL;
  cr_assert_null(s, "s should be NULL after assignment");
}

Test(examples, double_free) {
  char *p1 = (char *)malloc(16);
  strcpy(p1, "Hello, world!");
  cr_assert_str_eq(p1, "Hello, world!");
  char *p2 = p1;
  cr_assert_str_eq(p2, "Hello, world!");
  free(p1);
  /* cr_assert_str_eq(p2, "Hello, world!"); */ /* ERROR: AddressSanitizer: heap-use-after-free */
  /* free(p2); */ /* ERROR: AddressSanitizer: attempting double-free */
}

Test(examples, dangling_pointer_assignment) {
  int *pi = (int *)malloc(sizeof(int));
  *pi = 42;
  cr_assert_eq(*pi, 42);
  free(pi);
  /* *pi = 43; */ /* ERROR: AddressSanitizer: heap-use-after-free */
}

Test(examples, dangling_double_pointer) {
  int *p1 = (int *)malloc(sizeof(int));
  *p1 = 42;
  int *p2 = p1;
  cr_assert_eq(*p1, 42);
  cr_assert_eq(*p2, 42);
  
  *p2 = 43;
  cr_assert_eq(*p1, 43);
  cr_assert_eq(*p2, 43);
  
  free(p1);
  /* *p2 = 44; */ /* ERROR: AddressSanitizer: heap-use-after-free */
}

Test(examples, dangling_pointer_block_allocation) {
  int *pi;
  {
    int local = 42;
    pi = &local;
    cr_assert_eq(*pi, 42);
  }
  
  char buffer[128];
  strcpy(buffer, "calling a function after the local variable goes out of scope");
  /* cr_assert_eq(*pi, 42); */ /* ERROR: AddressSanitizer: stack-use-after-scope */
  /* *pi = 43; */ /* ERROR: AddressSanitizer: stack-use-after-scope */
}

/* RAII helper for char* pointers */
static inline void cleanup_ptr(void *p) {
  void **pp = (void **)p;
  if (*pp) free(*pp);
}

#define RAII_ALLOC(type, var, init_code) \
    type var __attribute__((cleanup(cleanup_ptr))) = (init_code)

Test(examples, raii_example) {
  RAII_ALLOC(char*, name, (char*)malloc(32));
  strcpy(name, "RAII Example");
  cr_assert_str_eq(name, "RAII Example");
}

Test(examples, raii_with_block) {
  char *name = NULL;
  {
    RAII_ALLOC(char*, name, (char*)malloc(32));
    strcpy(name, "RAII Example");
    cr_assert_str_eq(name, "RAII Example");
    // cleanup_ptr is called here when exiting the block, freeing the memory allocated for name
  }
  // name was freed
  cr_assert_null(name, "name should be NULL after exiting the block");
}

Test(examples, multiple_raii) {
  {
    RAII_ALLOC(char*, name1, (char*)malloc(32));
    strcpy(name1, "First");
    // cleanup_ptr called here for name1
  }
  
  {
    RAII_ALLOC(char*, name2, (char*)malloc(32));
    strcpy(name2, "Second");
    // cleanup_ptr called here for name2
  }
}

Test(examples, early_exit) {
  char *name = NULL;
  {
    RAII_ALLOC(char*, name, (char*)malloc(32));
    if (true) {
      return;  // name is still freed automatically
    }
    strcpy(name, "text");
  }
  cr_assert_null(name, "name should be NULL after early exit");
}
