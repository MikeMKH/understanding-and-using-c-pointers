#include "dynamic_alloc.h"
#include <stdlib.h>
#include <string.h>

void *safe_malloc(size_t size) {
  void *p = malloc(size);
  if (!p) abort();
  return p;
}

int *copy_int_array(const int *src, size_t n) {
  int *dst = safe_malloc(n * sizeof(int));
  memcpy(dst, src, n * sizeof(int));
  return dst;
}