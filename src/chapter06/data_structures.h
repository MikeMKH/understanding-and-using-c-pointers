#pragma once
#include <stddef.h>

typedef struct _employee {
  char name[32];
  unsigned char age;
} Employee;

int compare_employees(const Employee *e1, const Employee *e2);
void format_employee(const Employee *e, char *buffer, size_t buffer_size);

typedef void (*COMPARE)(Employee *e1, Employee *e2);
typedef void (*FORMAT)(Employee *e, char *buffer, size_t buffer_size);
