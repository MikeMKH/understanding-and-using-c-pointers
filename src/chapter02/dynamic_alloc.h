#pragma once
#include <stddef.h>

// Safe malloc: aborts on OOM rather than returning NULL silently
void *safe_malloc(size_t size);

// Returns a heap-allocated copy of n ints from src
int  *copy_int_array(const int *src, size_t n);