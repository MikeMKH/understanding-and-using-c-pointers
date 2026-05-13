#include <criterion/criterion.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_mutex_t.h>
#include <sys/_pthread/_pthread_t.h>
#include <pthread/pthread.h>

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

Test(example, aliasing) {
  int value = 8;
  int *p1 = &value;
  int *p2 = &value;
  
  *p1 = 10;
  cr_assert_eq(*p2, 10, "p1 and p2 alias the same value");
}

Test(example, strict_aliasing) {
  float value = 3.14f;
  unsigned int *p = (unsigned int*)&value;
  unsigned int positive = (*p & 0x80000000) == 0; /* faster than seeing if value > 0 */
  cr_assert_eq(positive, 1, "the float 3.14 is positive");
}

typedef union _conversion {
  float f;
  unsigned int u;
} Conversion;

int is_positive(float f) {
  Conversion c = { .f = f };
  return (c.u & 0x80000000) == 0;
}

Test(is_positive, should_return_true_for_positive_float) {
  cr_assert(is_positive(3.14f));
  cr_assert(is_positive(0.0f));
}

Test(is_positive, should_return_false_for_negative_float) {
  cr_assert_not(is_positive(-3.14f));
}

typedef union _not_strict_aliasing {
  float *f;
  unsigned int *u;
} Conversion2;

int is_positive2(float f) {
  /* not strict aliasing because of the pointers */
  Conversion2 c = { .f = &f };
  return (*c.u & 0x80000000) == 0;
}

Test(is_positive2, should_return_true_for_positive_float_but_is_not_strict_aliased) {
  cr_assert(is_positive2(3.14f));
  cr_assert(is_positive2(0.0f));
}

Test(is_positive2, should_return_false_for_negative_float_but_is_not_strict_aliased) {
  cr_assert_not(is_positive2(-3.14f));
}

int is_positive3(float f) {
  /* not strict aliasing because of the pointers */
  unsigned int *p = (unsigned int*) &f;
  return (*p & 0x80000000) == 0;
}

Test(is_positive3, should_return_true_for_positive_float_but_is_not_strict_aliased) {
  cr_assert(is_positive3(3.14f));
  cr_assert(is_positive3(0.0f));
}

Test(is_positive3, should_return_false_for_negative_float_but_is_not_strict_aliased) {
  cr_assert_not(is_positive3(-3.14f));
}

void add(int size, double * restrict arr1, double * restrict arr2) {
  /* restrict tells the compiler that these pointers will not alias each other */
  for (int i = 0; i < size; i++) {
    arr1[i] += arr2[i];
  }
}

Test(examples, using_restrict_to_optimize_add) {
  int size = 3;
  double arr1[] = {1.0, 2.0, 3.0};
  double arr2[] = {4.0, 5.0, 6.0};
  
  add(size, arr1, arr2);
  
  cr_assert_eq(arr1[0], 5.0);
  cr_assert_eq(arr1[1], 7.0);
  cr_assert_eq(arr1[2], 9.0);
}

typedef struct {
  double *vectorA;
  double *vectorB;
  double sum;
  int length;
} VectorInfo;

typedef struct {
  VectorInfo *info;
  int beginningIndex;
} Product;

pthread_mutex_t mutexSum;

void *dotProduct(void *prod) {
  Product *product = (Product*)prod;
  VectorInfo *vectorInfo = product->info;
  int beginningIndex = product->beginningIndex;
  int endingIndex = beginningIndex + vectorInfo->length;
  double total = 0.0;
  
  for (int i = beginningIndex; i < endingIndex; i++) {
    total += vectorInfo->vectorA[i] * vectorInfo->vectorB[i];
  }
  
  pthread_mutex_lock(&mutexSum);
  vectorInfo->sum += total;
  pthread_mutex_unlock(&mutexSum);
  
  return (void*)0;
}

#define NUM_THREADS 4

Test(examples, using_pthreads_for_dot_product) {
  VectorInfo vectorInfo;
  double vectorA[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
  double vectorB[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};

  vectorInfo.vectorA = vectorA;
  vectorInfo.vectorB = vectorB;
  vectorInfo.length = 4;
  pthread_t threads[NUM_THREADS];
  void *status;
  pthread_attr_t attr;
  pthread_mutex_init(&mutexSum, NULL);
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  int returnValue;
  int threadNumber;
  
  for (threadNumber = 0; threadNumber < NUM_THREADS; threadNumber++) {
    Product *product = malloc(sizeof(Product));
    product->beginningIndex = threadNumber * vectorInfo.length;
    product->info = &vectorInfo;
    returnValue = pthread_create(&threads[threadNumber], &attr, dotProduct, (void*)product);
    if (returnValue) {
      cr_expect_fail("pthread_create failed with error code %d", returnValue);
    }
  }
  
  pthread_attr_destroy(&attr);
  for (threadNumber = 0; threadNumber < NUM_THREADS; threadNumber++) {
    pthread_join(threads[threadNumber], &status);
  }
  pthread_mutex_destroy(&mutexSum);
  cr_assert_eq(vectorInfo.sum, 1496.0);
}