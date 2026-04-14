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

typedef int (*getchar_func)(void);

char *__getLine(getchar_func _getchar) {
  const size_t sizeIncrement  = 10;
  char *buffer = malloc(sizeIncrement);
  char *currentPosition = buffer;
  size_t maximumLength = sizeIncrement;
  size_t length = 0;
  int character;
  
  if (currentPosition == NULL) { return NULL; /* malloc failed */ }
  
  while(1) {
    character = _getchar();
    if (character == EOF || character == '\n') { break; }
    
    if (length + 1 >= maximumLength) {  /* +1 to reserve space for null terminator */
      maximumLength += sizeIncrement;
      char *newBuffer = realloc(buffer, maximumLength);
      if (newBuffer == NULL) {
        free(buffer); /* free the old buffer if realloc fails */
        return NULL; /* realloc failed */
      }
      currentPosition = newBuffer + (currentPosition - buffer);
      buffer = newBuffer;
    }
    *currentPosition++ = character;
    length++;
  }
  *currentPosition = '\0';
  return buffer;
}
char *getLine(void) { return __getLine(&getchar); }

static char *mock_input = NULL;
static int mock_index = 0;
static int mock_getchar(void) {
  if (mock_input == NULL || mock_input[mock_index] == '\0') { return EOF; }
  return mock_input[mock_index++];
}

static void setup(void) {
  mock_input = NULL;
  mock_index = 0;
}

TestSuite(__getLine, .init = setup);

Test(__getLine, reads_simple_string) {
  mock_input = "hello";
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, "hello");
  free(result);
}

Test(__getLine, reads_empty_string) {
  mock_input = "";
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, "");
  free(result);
}

Test(__getLine, reads_single_character) {
  mock_input = "a";
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, "a");
  free(result);
}

Test(__getLine, stops_at_newline) {
  mock_input = "hello\nworld";
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, "hello");
  free(result);
}

Test(__getLine, expands_buffer_for_long_input) {
  // 100 chars exceeds initial 10-char buffer
  mock_input = "aaaaaaaaaa"  // 10
               "bbbbbbbbbb"  // 20
               "cccccccccc"  // 30
               "dddddddddd"  // 40
               "eeeeeeeeee"; // 50
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, mock_input);
  free(result);
}

Test(__getLine, handles_multiple_expansions) {
  // Over 100 chars to force multiple buffer resizes
  mock_input = "aaaaaaaaaa"  // 10
               "bbbbbbbbbb"  // 20
               "cccccccccc"  // 30
               "dddddddddd"  // 40
               "eeeeeeeeee"  // 50
               "ffffffffff"  // 60
               "gggggggggg"  // 70
               "hhhhhhhhhh"  // 80
               "iiiiiiiiii"  // 90
               "jjjjjjjjjj"; // 100
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, mock_input);
  free(result);
}

Test(__getLine, handles_eof_without_newline) {
  mock_input = "test";
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, "test");
  free(result);
}

Test(__getLine, handles_special_characters) {
  mock_input = "hello\t\tworld  123!@#";
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, "hello\t\tworld  123!@#");
  free(result);
}

Test(__getLine, handles_null_mock_input) {
  mock_input = NULL;
  char *result = __getLine(mock_getchar);
  cr_assert_str_eq(result, "");
  free(result);
}

char* trim(char* phrase) {
  char* old = phrase;
  char* new = phrase;
  
  /* trim leading spaces */
  while (*old == ' ') { old++; }
  
  /* copy characters until end of string */
  while (*old) { *(new++) = *(old++); }
  
  /* trim trailing spaces */
  while (new > phrase && *(new - 1) == ' ') { new--; }
  *new = '\0';
  
  return  (char*)realloc(phrase, strlen(phrase)+1);
}

Test(trim, removes_leading_spaces) {
  char *input = strdup("   hello world");
  char *result = trim(input);
  cr_assert_str_eq(result, "hello world");
  free(result);
}

Test(trim, removes_trailing_spaces) {
  char *input = strdup("hello world    ");
  char *result = trim(input);
  cr_assert_str_eq(result, "hello world");
  free(result);
}

Test(trim, removes_both_leading_and_trailing_spaces) {
  char *input = strdup("   hello world    ");
  char *result = trim(input);
  cr_assert_str_eq(result, "hello world");
  free(result);
}

Test(trim, does_not_modify_original_string_if_no_leading_or_trailing_spaces) {
  char *input = strdup("hello world");
  char *result = trim(input);
  cr_assert_str_eq(result, "hello world");
  free(result);
}

Test(examples, array_of_strings_array_notation) {
  const char *strings[] = {"hello", "world", "test"};
  cr_assert_str_eq(strings[0], "hello");
  cr_assert_str_eq(strings[1], "world");
  cr_assert_str_eq(strings[2], "test");
}

Test(examples, array_of_strings_pointer_notation) {
  const char *strings[] = {"hello", "world", "test"};
  cr_assert_str_eq(*(strings + 0), "hello");
  cr_assert_str_eq(*(strings + 1), "world");
  cr_assert_str_eq(*(strings + 2), "test");
}

Test(examples, array_of_strings_both_array_and_pointer_notation) {
  const char *strings[] = {"hello", "world", "test"};
  for (int i = 0; i < 3; i++) {
    cr_assert_str_eq(strings[i], *(strings + i));
  }
}

Test(examples, array_of_pointers) {
  int *arr[5];
  for (int i = 0; i < 5; i++) {
    arr[i] = malloc(sizeof(int));
    *arr[i] = i + 1;
  }
  
  cr_assert_eq(*arr[0], 1);
  cr_assert_eq(**(arr+1), 2);
  cr_assert_eq(**(arr+1), *arr[1]);
  cr_expect(arr[0] < arr[1], "first pointer should be at a lower memory address than second pointer");
  
  for (int i = 0; i < 5; i++) {
    cr_assert_eq(*arr[i], i + 1);
    free(arr[i]);
  }
}

Test(examples, each_row_of_multidimensional_array_is_a_one_dimensional_array) {
  int matrix[2][5] = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10}
  };
  
  cr_assert_eq(sizeof(matrix[0]), sizeof(int) * 5);
  cr_assert_eq(sizeof(matrix[1]), sizeof(int) * 5);
  cr_assert_eq(**(matrix + 1), matrix[1][0]);
  cr_assert_eq(matrix[0][0], 1);
  cr_assert_eq(matrix[1][0], 6);
  cr_assert_eq(*matrix[0] + 1, *(matrix[0] + 1));
  cr_expect(&matrix[0] < &matrix[1], "first row should be at a lower memory address than second row");
  cr_expect(matrix < matrix + 1, "first row should be at a lower memory address than second row");
  
  int *p = matrix[0];
  cr_assert_eq(*p, matrix[0][0]);
  cr_assert_eq(*p + 1, matrix[0][1]);
  cr_expect_eq(*p + 5, matrix[1][0], "pointer arithmetic should move to the next row after 5 elements");
  
  cr_expect_eq(sizeof(matrix[0]), 20, "each row should be 5 integers, so 20 bytes total");
  cr_expect_eq(sizeof(matrix), 40, "matrix should be 2 rows of 5 integers each, so 40 bytes total");
}

void assertSequence(int arr[][5], int rows) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < 5; j++) {
      const int expected = i * 5 + j + 1;
      cr_assert_eq(arr[i][j], expected);
    }
  }
}
Test(examples, passing_a_multidimensional_array) {
  int matrix[2][5] = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10}
  };
  assertSequence(matrix, 2);
  assertSequence((int (*)[5])&matrix[0][0], 2);
}

void assertSequenceUnknownSize(int *arr, int rows, int columns) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      const int position = (i * columns) + j;
      const int expected = position + 1;
      cr_assert_eq(*(arr + position), expected);
    }
  }
}
Test(examples, passing_a_multidimensional_array_of_unknown_size) {
  int matrix[2][5] = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10}
  };
  assertSequenceUnknownSize(&matrix[0][0], 2, 5);
  assertSequenceUnknownSize((int *)matrix, 2, 5);
}

Test(examples, stack_allocated_two_dimensional_array) {
  int matrix[2][5] = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10}
  };
  cr_assert_eq(matrix[0][0], 1);
  cr_assert_eq(matrix[1][0], 6);
  cr_expect(&matrix[0] < &matrix[1], "first row should be at a lower memory address than second row");
  cr_expect(&matrix[1][0] < &matrix[1][1], "first element of a row should be at a lower memory address than second element of a row");
}

Test(examples, heap_allocated_two_dimensional_array_non_contiguous_memory) {
  int rows = 2;
  int columns = 5;
  int **matrix = malloc(rows * sizeof(int *));
  for (int i = 0; i < rows; i++) {
    matrix[i] = malloc(columns * sizeof(int));
  }
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      matrix[i][j] = (i * columns) + j + 1;
    }
  }
  
  cr_assert_eq(matrix[0][0], 1);
  cr_assert_eq(matrix[1][0], 6);
  cr_expect(&matrix[0] < &matrix[1], "first row might be at a lower memory address than second row");
  cr_expect(&matrix[1][0] < &matrix[1][1], "first element of a row should be at a lower memory address than second element of a row");
  
  for (int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

Test(examples, heap_allocated_two_dimensional_array_contiguous_memory_with_two_malloc) {
  int rows = 2;
  int columns = 5;
  int **matrix = malloc(rows * sizeof(int *));
  matrix[0] = malloc(rows * columns * sizeof(int));
  
  for (int i = 0; i < rows; i++) {
    matrix[i] = matrix[0] + (i * columns);
  }
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      matrix[i][j] = (i * columns) + j + 1;
    }
  }
  
  cr_assert_eq(matrix[0][0], 1);
  cr_assert_eq(matrix[1][0], 6);
  cr_expect(&matrix[0] < &matrix[1], "first row should be at a lower memory address than second row");
  cr_expect(&matrix[1][0] < &matrix[1][1], "first element of a row should be at a lower memory address than second element of a row");
  
  free(matrix[0]);
  free(matrix);
}

Test(examples, heap_allocated_two_dimensional_array_contiguous_memory_with_single_malloc) {
  int rows = 2;
  int columns = 5;
  int *matrix = malloc(rows * columns * sizeof(int));
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      *(matrix + (i * columns) + j) = (i * columns) + j + 1;
    }
  }
  
  cr_assert_eq(*(matrix + 0), 1);
  cr_assert_eq(*(matrix + columns), 6);
  cr_assert_eq(*(matrix + (columns + 1)), 7);
  cr_expect(matrix < matrix + columns, "first row should be at a lower memory address than second row");
  cr_expect(matrix + columns < matrix + (columns + 1), "first element of a row should be at a lower memory address than second element of a row");
  
  free(matrix);
}