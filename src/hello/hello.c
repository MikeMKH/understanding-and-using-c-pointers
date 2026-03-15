#include "hello.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *hello(const char *name) {
  // "Hello " (6) + name + "!" (1) + null terminator (1)
  size_t len = 6 + strlen(name) + 2;
  char *result = malloc(len);
  if (!result) abort();
  snprintf(result, len, "Hello %s!", name);
  return result;
}