#include <criterion/criterion.h>
#include <stdlib.h>

Test(examples, address_of_operator) {
  int num = 100;
  int *pi = &num;
  cr_assert_eq(*pi, 100);
  cr_assert_neq(pi, (int *)100, "congratulations, you have a pointer in lower memory!");
}

Test(examples, assignment_of_value) {
  int num = 100;
  int *pi = &num;
  *pi = 200;
  cr_assert_eq(num, 200);
}

Test(examples, pointer_to_void) {
  int num = 100;
  void *pv = &num;
  cr_assert_eq(*(int *)pv, 100);
  *(int *)pv = 200;
  cr_assert_eq(num, 200);
}

Test(examples, const_static_pointer_init_null) {
  /* text has just const int *pi; but gives a warning */
  const int *pi = NULL;
  cr_assert_eq(pi, NULL);
}

Test(examples, size_t_given_negative_value) {
  size_t x = -5;
  cr_assert_eq(x, (size_t)-5);
  
  char buffer[24];
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wformat"
  snprintf(buffer, sizeof(buffer), "%d", x);
  #pragma GCC diagnostic pop
  cr_assert_str_eq(buffer, "-5");
  
  snprintf(buffer, sizeof(buffer), "%zu", x);
  cr_assert_str_eq(buffer, "18446744073709551611");
}

Test(examples, pointer_addition_access_array_members) {
  int vector[] = {28, 41, 7};
  
  int *pi = vector;
  cr_assert_eq(*pi, 28);
  cr_assert_eq(pi, (int*)&vector,"pointer should point to vector + 0");
  
  pi += 1;
  cr_assert_eq(*pi, 41);
  cr_assert_eq(pi, (int*)((char*)&vector + sizeof(int)), "pointer should point to vector + 1");
  
  pi++;
  cr_assert_eq(*pi, 7);
  cr_assert_eq(pi, (int*)((char*)&vector + 2 * sizeof(int)), "pointer should point to vector + 2");
  
  pi += 1;
  cr_assert_eq(pi, (int*)((char*)&vector + 3 * sizeof(int)), "can continues past the end of the array");
  pi++;
  cr_assert_eq(pi, (int*)((char*)&vector + 4 * sizeof(int)), "... and so on");
}

Test(examples, pointer_subtraction) {
  int vector[] = {28, 41, 7};
  
  int *pi = vector + 2;
  cr_assert_eq(*pi, 7);
  
  pi -= 1;
  cr_assert_eq(*pi, 41);
  
  pi--;
  cr_assert_eq(*pi, 28);
}

Test(examples, pointer_comparison) {
  int vector[] = {28, 41, 7};
  
  int *p0 = vector;
  int *p1 = vector + 1;
  int *p2 = vector + 2;
  
  cr_assert_eq(p1 - p0, 1);
  cr_assert_eq(p2 - p0, 2);
  cr_assert_eq(p0 - p1, -1);
  
  cr_assert_eq(*p0 - *p1, -13, "can still deference pointers to get values");
  
  cr_assert(p0 + 1 == p1);
  cr_assert(p1 < p2);
  cr_assert(p2 > p1);
  cr_assert(p0 <= p0);
  cr_assert(p1 >= p0);
  cr_assert(p2 != p1);
}

Test(examples, multiple_levels_of_indirection) {
  char *titles[] = {
    "A Tale of Two Cities",
    "Wuthering Heights",
    "Don Quixote",
    "Odyssey",
    "Moby-Dick",
    "Hamlet",
    "Gulliver's Travels"
  };
  
  char **bestBooks[3];
  bestBooks[0] = &titles[0];
  bestBooks[1] = &titles[3];
  bestBooks[2] = &titles[5];
  
  char **englishBooks[4];
  englishBooks[0] = &titles[0];
  englishBooks[1] = &titles[1];
  englishBooks[2] = &titles[5];
  englishBooks[3] = &titles[6];
  
  cr_assert_str_eq(titles[0], "A Tale of Two Cities");
  cr_assert_str_eq(*bestBooks[0], "A Tale of Two Cities");
  cr_assert_str_eq(*englishBooks[0], "A Tale of Two Cities");
  cr_assert_eq(titles, bestBooks[0]);
  cr_assert_eq(englishBooks[0], bestBooks[0]);
  
  cr_assert_str_eq(titles[1], "Wuthering Heights");
  cr_assert_str_eq(*bestBooks[1], "Odyssey");
  cr_assert_str_eq(*englishBooks[1], "Wuthering Heights");
}

Test(examples, const_pointer_to_const) {
  int num = 5;
  const int limit = 100;
  int *pi;
  const int *pci;
  
  pi = &num;
  pci = &limit;
  
  cr_assert_eq(*pi, 5);
  cr_assert_eq(*pci, 100);
  
  pci = &num;
  cr_assert_eq(*pci, 5);
  /* *pci = 200; */ /* error: assignment of read-only location */
}

Test(examples, const_pointer_to_non_constant) {
  int num;
  const int limit = 100;
  int *const cpi = &num;
  
  cr_assert_eq(*cpi, num);
  
  *cpi = limit;
  cr_assert_eq(*cpi, 100);
  
  *cpi = 25;
  cr_assert_eq(*cpi, 25);
  
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
  int *const warn = &limit; /* discards qualifiers */
  #pragma GCC diagnostic pop
  cr_assert_eq(*warn, 100);
  
  /* cpi = &num; */ /* error: cannot assign to variable 'cpi' with const-qualified type 'int *const' */
}

Test(examples, constant_pointer_to_constant) {
  const int limit = 500;
  const int * const cpci = &limit;
  
  cr_assert_eq(*cpci, 500);
  cr_assert_eq(*cpci, limit);
  cr_assert_eq(cpci, &limit);
  
  const int * const *pcpci = &cpci;
  cr_assert_eq(**pcpci, 500);
  cr_assert_eq(*pcpci, cpci);
  /* *cpci = 1000; */ /* error: assignment of read-only location */
  
  int num = 25;
  const int * const p = &num;
  pcpci = &p;
  cr_assert_eq(**pcpci, 25);
  pcpci = &cpci;
  cr_assert_eq(**pcpci, 500);
}
