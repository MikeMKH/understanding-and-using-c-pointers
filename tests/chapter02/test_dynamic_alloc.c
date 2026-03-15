#include <criterion/criterion.h>
#include "dynamic_alloc.h"
#include <stdlib.h>

Test(copy_int_array, preserves_values) {
    int src[] = {10, 20, 30};
    int *dst = copy_int_array(src, 3);

    cr_assert_eq(dst[0], 10);
    cr_assert_eq(dst[1], 20);
    cr_assert_eq(dst[2], 30);

    free(dst);
}

Test(copy_int_array, independent_copy) {
    int src[] = {1, 2, 3};
    int *dst = copy_int_array(src, 3);

    dst[0] = 99;
    cr_assert_eq(src[0], 1, "mutation of copy must not affect source");

    free(dst);
}