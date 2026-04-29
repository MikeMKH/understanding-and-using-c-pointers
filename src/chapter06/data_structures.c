#include "data_structures.h"
#include <string.h>
#include <stdio.h>

int compare_employees(const Employee *e1, const Employee *e2) {
  return strcmp(e1->name, e2->name);
}

void format_employee(const Employee *e, char *buffer, size_t buffer_size) {
  snprintf(buffer, buffer_size, "Name: %s, Age: %u", e->name, e->age);
}