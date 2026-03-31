#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

int assert_adder(int a, int b) {
  int result = a + b;
  cr_expect(&a < &b, "first argument should be at a lower memory address than second argument");
  /*
    ARM64 Local variables grow downward but don't necessarily have predictable ordering relative 
    to register-passed arguments like they do on x86-64, so we can't reliably assert the ordering 
    of local variables relative to arguments on ARM64
  */
  cr_expect(&result > &a, "local variable should be at a higher memory address than arguments");
  return result;
}

Test(examples, organization_of_program_stack) {
  int accumulator = 0;
  for(int i = 0; i < 10; i += 2) {
    int sum = assert_adder(i, i+1);
    cr_expect(&sum > &accumulator, "block variable should be at a lower memory address than local variable");
    accumulator += sum;
  }
  cr_assert_eq(accumulator, 45, "the sum of [0, 9] is 45");
}

void swapWithPointers(int *pi1, int *pi2) {
  int temp = *pi1;
  *pi1 = *pi2;
  *pi2 = temp;
}

Test(examples, swap_with_pointers) {
  int a = 5;
  int b = 10;
  swapWithPointers(&a, &b);
  cr_assert_eq(a, 10, "a should be swapped to 10");
  cr_assert_eq(b, 5, "b should be swapped to 5");
}

void swapWithPassByValue(int num1, int num2) {
  /* does NOT actually swap the values of the arguments */
  int temp = num1;
  num1 = num2;
  num2 = temp;
}

Test(examples, swap_with_pass_by_value) {
  int a = 5;
  int b = 10;
  swapWithPassByValue(a, b);
  cr_assert_eq(a, 5, "a should remain unchanged at 5");
  cr_assert_eq(b, 10, "b should remain unchanged at 10");
}

void passingAddressOfConstants(const int *cpi, int* num) {
  *num = *cpi; /* this is fine, we can read from the constant */
  /* *cpi += 5; */ /* error: read-only variable is not assignable */
}

Test(examples, passing_address_of_constants) {
  const int constant = 10;
  int result = 0;
  passingAddressOfConstants(&constant, &result);
  cr_assert_eq(result, 10, "result should be 10 after reading from the constant");
}

int* allocatingArray(int size, int initialValue) {
  int *array = malloc(size * sizeof(int));
  for (int i = 0; i < size; i++) array[i] = initialValue;
  return array;
}

Test(examples, allocating_array) {
  int size = 5;
  int value = 42;
  int *array = allocatingArray(size, value);
  for (int i = 0; i < size; i++) {
    cr_assert_eq(array[i], value, "array element should be initialized to the initial value");
  }
  free(array); /* caller must free the allocated memory */
}

int* allocatingArrayWithLocalMemory(int size, int initialValue) {
  /* this array is allocated on the stack and will go out of scope when the function returns */
  int array[size];
  for (int i = 0; i < size; i++) array[i] = initialValue;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-stack-address"
  return array; /* warning: address of stack memory associated with local variable 'array' returned */
#pragma GCC diagnostic pop
}

Test(examples, allocating_array_with_local_memory) {
  int size = 5;
  int value = 42;
  int *array = allocatingArrayWithLocalMemory(size, value);
  /* accessing array here is undefined behavior since it points to memory that has gone out of scope */
  for(int i = 0; i < size; i++) {
    /* we can't reliably assert the values here since it's undefined behavior, but we can at least check that the pointer is not NULL */
    cr_expect_not_null(array, "array pointer should not be NULL even though it points to out-of-scope memory");
  }
}

void allocatingArrayWithPassedInPointer(int* array, int size, int initialValue) {
  if (array == NULL) return; /* caller must ensure array is allocated */
  for (int i = 0; i < size; i++) array[i] = initialValue;
}

Test(examples, allocating_array_with_passed_in_pointer) {
  int size = 5;
  int value = 42;
  
  int array1[size]; /* caller allocates the array */
  allocatingArrayWithPassedInPointer(array1, size, value);
  for (int i = 0; i < size; i++) {
    cr_assert_eq(array1[i], value, "array element should be initialized to the initial value");
  }
  
  int *array2 = malloc(size * sizeof(int)); /* caller allocates the array */
  allocatingArrayWithPassedInPointer(array2, size, value);
  for (int i = 0; i < size; i++) {
    cr_assert_eq(array2[i], value, "array element should be initialized to the initial value");
  }
  free(array2);
}

void allocatingArrayWithPointerToPointer(int** array, int size, int initialValue) {
  *array = malloc(size * sizeof(int)); /* function allocates the array and updates the caller's pointer */
  for (int i = 0; i < size; i++) (*array)[i] = initialValue;
}

Test(examples, allocating_array_with_pointer_to_pointer) {
  int size = 5;
  int value = 42;
  
  int *array1; /* caller declares a pointer but does not allocate the array */
  allocatingArrayWithPointerToPointer(&array1, size, value);
  for (int i = 0; i < size; i++) {
    cr_assert_eq(array1[i], value, "array element should be initialized to the initial value");
  }
  free(array1);
  
  int *array2 = NULL; /* caller initializes pointer to NULL */
  allocatingArrayWithPointerToPointer(&array2, size, value);
  for (int i = 0; i < size; i++) {
    cr_assert_eq(array2[i], value, "array element should be initialized to the initial value");
  }
  free(array2);
}

void saferFree(void **ptr) {
  if (ptr != NULL && *ptr != NULL) {
    free(*ptr);
    *ptr = NULL; /* set caller's pointer to NULL to prevent dangling pointer */
  }
}
#define saferFree(p) saferFree((void**)&(p))

Test(examples, safer_free) {
  int *pi = malloc(5 * sizeof(int));
  saferFree(pi);
  cr_assert_null(pi, "pointer should be set to NULL after saferFree");
  
  char *str = malloc(20 * sizeof(char));
  strcpy(str, "Hello, world!");
  cr_assert_str_eq(str, "Hello, world!");
  saferFree(str);
  cr_assert_null(str, "pointer should be set to NULL after saferFree");
}

int square(int x) {
  return x * x;
}

Test(examples, function_pointer_declaration_and_usage) {
  int (*fptr)(int) = square; /* or: int (*funcPtr)(int) = &square; */
  cr_assert_not_null(fptr);
  int result = fptr(5);
  cr_assert_eq(result, 25);
}

typedef int (*funcptr)(int);

Test(examples, function_pointer_using_typedef) {
  funcptr fptr = square;
  cr_assert_not_null(fptr);
  int result = fptr(6);
  cr_assert_eq(result, 36);
}

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

typedef int (*binary_op)(int, int);

int compute(binary_op op, int x, int y) { return op(x, y); }

Test(examples, function_pointer_as_argument) {
  cr_assert_eq(compute(add, 5, 3), 8);
  cr_assert_eq(compute(sub, 5, 3), 2);
  cr_assert_eq(compute(mul, 5, 3), 15);
}

binary_op select(char operator) {
  switch (operator) {
    case '+': return add;
    case '-': return sub;
    case '*': return mul;
    default : return NULL;
  }
}

int evaluate(char operator, int x, int y) {
  binary_op op = select(operator);
  return (op != NULL)
    ? op(x, y)
    : 0;
}

Test(examples, function_pointer_as_return_value) {
  cr_assert_eq(evaluate('+', 5, 3), 8);
  cr_assert_eq(evaluate('-', 5, 3), 2);
  cr_assert_eq(evaluate('*', 5, 3), 15);
  cr_assert_eq(evaluate('/', 5, 3), 0, "unsupported operator should return 0");
}

binary_op operations[128] = { NULL };

void init_operations(void) {
  operations['+'] = add;
  operations['-'] = sub;
  operations['*'] = mul;
}

int dispatch(char operator, int x, int y) {
  binary_op op = operations[(int)operator];
  return (op != NULL)
    ? op(x, y)
    : 0;
}

Test(examples, function_pointer_in_array) {
  init_operations();
  cr_assert_eq(dispatch('+', 5, 3), 8);
  cr_assert_eq(dispatch('-', 5, 3), 2);
  cr_assert_eq(dispatch('*', 5, 3), 15);
  cr_assert_eq(dispatch('/', 5, 3), 0, "unsupported operator should return 0");
}

Test(examples, compare_function_pointers) {
  binary_op add_ptr = add;
  binary_op sub_ptr = sub;
  cr_assert(add_ptr == add);
  cr_assert(sub_ptr == sub);
  cr_assert(add_ptr != sub_ptr);
}

Test(examples, casting_function_pointers) {
  void (*void_ptr)(void) = (void (*)(void)) add; /* cast add to a generic function pointer type */
  cr_assert_not_null(void_ptr);
  /*
     we can't call void_ptr directly since it doesn't have the correct type,
    but we can cast it back to the original type
  */
  int (*add_ptr)(int, int) = (int (*)(int, int)) void_ptr;
  cr_assert_eq(add_ptr(5, 3), 8);
}
