#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, one_dimensional_array) {
  int vector[5] = {1, 2, 3, 4, 5};
  cr_assert_eq(sizeof(vector), 5 * sizeof(int));
  cr_assert_eq(sizeof(vector)/sizeof(int), 5, "length = total size / size of element");
  cr_assert_eq(*vector, 1);
  cr_assert_eq(vector[0], 1);
  cr_assert_eq(vector[1], 2);
  cr_assert_eq(vector[2], 3);
  cr_assert_eq(vector[3], 4);
  cr_assert_eq(vector[4], 5);
}

Test(examples, two_dimensional_array) {
  int matrix[2][3] = {
    {1, 2, 3},
    {4, 5, 6}
  };
  cr_assert_eq(sizeof(matrix[0]), sizeof(matrix[1]));
  cr_expect(&matrix[0] < &matrix[1], "first row should be at a lower memory address than second row");
  cr_assert_eq(sizeof(matrix)/sizeof(int), 2 * 3, "length = total size / size of element");
  cr_assert_eq(matrix[0][0], 1);
  cr_assert_eq(matrix[1][0], 4);
}

Test(examples, multidimensional_array) {
  int arr3d[3][2][4] = {
    {{ 1,  2,  3,  4}, { 5,  6,  7,  8}},
    {{ 9, 10, 11, 12}, {13, 14, 15, 16}},
    {{17, 18, 19, 20}, {21, 22, 23, 24}}
  };
  cr_expect(&arr3d[0][0][0] < &arr3d[1][0][0], "first 2D slice should be at a lower memory address than second 2D slice");
  cr_assert_eq(arr3d[0][0][0], 1);
  cr_assert_eq(arr3d[1][0][0], 9);
  cr_assert_eq(arr3d[2][1][3], 24);
}

Test(examples, array_name_points_to_first_element) {
  int vector[] = {28, 41, 7};
  cr_assert_eq(vector, &vector[0], "array name should point to the first element of the array");
  cr_assert_eq(*vector, 28);
}

Test(examples, arrays_can_use_pointer_notation) {
  int vector[] = {28, 41, 7};
  int *pi = vector;
  
  cr_assert_eq(*pi, 28);
  cr_assert_eq(*(vector), 28);
  
  cr_assert_eq(*(pi + 1), 41);
  cr_assert_eq(*(vector + 1), 41);
  
  cr_assert_eq(vector, &vector[0]);
  cr_assert_eq(pi, &vector[0]);
  cr_assert_eq(&pi[0], vector);
}

Test(examples, array_names_are_not_lvalues) {
  int vector[] = {28, 41, 7};
  int *pi = vector;
  /* vector = vector + 1; */ /* error: array type 'int[3]' is not assignable */
  pi = vector + 1;
  cr_assert_eq(pi, &vector[1]);
}

Test(examples, using_malloc_to_create_arrays) {
  int *pv = malloc(5 * sizeof(int));
  cr_assert_not_null(pv, "malloc should return a non-null pointer");
  
  for (int i = 0; i < 5; i++) {
    pv[i] = i + 1;
  }
  
  cr_assert_eq(pv[0], 1);
  cr_assert_eq(pv[4], 5);
  
  for (int i = 0; i < 5; i++) {
    *(pv + i) = i + 1;
  }
  
  cr_assert_eq(pv[0], 1);
  cr_assert_eq(pv[4], 5);
  
  free(pv);
}