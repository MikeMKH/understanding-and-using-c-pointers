#include <criterion/criterion.h>
#include "data_structures.h"
#include <stdlib.h>

Test(employee, compare_employees) {
  Employee e1 = {"Alice", 30};
  Employee e2 = {"Bob", 25};
  Employee e3 = {"Alice", 30};
  
  cr_assert(compare_employees(&e1, &e2) < 0);
  cr_assert(compare_employees(&e2, &e1) > 0);
  cr_assert(compare_employees(&e1, &e3) == 0);
}

Test(employee, format_employee) {
  Employee e = {"Alice", 30};
  char buffer[64];
  
  format_employee(&e, buffer, sizeof(buffer));
  cr_assert_str_eq(buffer, "Name: Alice, Age: 30");
}