#include <criterion/criterion.h>
#include <stdlib.h>

/*
struct _person {
  char* firstName;
  char* lastName;
  char* title;
  unsigned short age;
};
*/

typedef struct _person {
  char* firstName;
  char* lastName;
  char* title;
  unsigned int age;
} Person;

Test(examples, person_allocated_on_stack) {
  Person p = {"John", "Doe", "Mr.", 30};
  
  cr_assert_str_eq(p.firstName, "John");
  cr_assert_str_eq(p.lastName, "Doe");
  cr_assert_str_eq(p.title, "Mr.");
  cr_assert_eq(p.age, 30);
}

Test(examples, person_allocated_on_stack_without_using_typedef) {
  struct _person p = {"John", "Doe", "Mr.", 30};
  
  cr_assert_str_eq(p.firstName, "John");
  cr_assert_str_eq(p.lastName, "Doe");
  cr_assert_str_eq(p.title, "Mr.");
  cr_assert_eq(p.age, 30);
}

Test(examples, person_allocated_on_heap) {
  Person *p = malloc(sizeof(Person));
  p->firstName = "John";
  p->lastName = "Doe";
  p->title = "Mr.";
  p->age = 30;
  
  cr_assert_str_eq(p->firstName, "John");
  cr_assert_str_eq(p->lastName, "Doe");
  cr_assert_str_eq(p->title, "Mr.");
  cr_assert_eq(p->age, 30);
  
  free(p);
}

typedef struct _person_using_short {
  char* firstName;
  char* lastName;
  char* title;
  unsigned short age;
} PersonShort;

Test(examples, person_using_short) {
  PersonShort p = {"John", "Doe", "Mr.", 30};
  
  cr_assert_str_eq(p.firstName, "John");
  cr_assert_str_eq(p.lastName, "Doe");
  cr_assert_str_eq(p.title, "Mr.");
  cr_assert_eq(p.age, 30);
}

Test(examples, short_and_int_take_same_amount_of_space_due_to_padding) {
  cr_expect_eq(sizeof(Person), sizeof(PersonShort), "due to padding, both structs have the same size");
}

Person* initialized_person(Person *p,
  const char* firstName, const char* lastName, const char* title, unsigned int age) {
  if (!p) { p = malloc(sizeof(Person)); }
  
  p->firstName = malloc(strlen(firstName) + 1);
  strcpy(p->firstName, firstName);
  p->lastName = malloc(strlen(lastName) + 1);
  strcpy(p->lastName, lastName);
  p->title = malloc(strlen(title) + 1);
  strcpy(p->title, title);
  p->age = age;
  return p;
}

void deallocate_person(Person *p) {
  free(p->firstName);
  free(p->lastName);
  free(p->title);
}

Test(examples, initialized_person_without_pointer) {
  Person *p = initialized_person(NULL, "John", "Doe", "Mr.", 30);
  
  cr_assert_str_eq(p->firstName, "John");
  cr_assert_str_eq(p->lastName, "Doe");
  cr_assert_str_eq(p->title, "Mr.");
  cr_assert_eq(p->age, 30);
  
  deallocate_person(p);
  free(p);
}

Test(examples, initialized_person_with_pointer) {
  Person *p = malloc(sizeof(Person));
  initialized_person(p, "John", "Doe", "Mr.", 30);
  
  cr_assert_str_eq(p->firstName, "John");
  cr_assert_str_eq(p->lastName, "Doe");
  cr_assert_str_eq(p->title, "Mr.");
  cr_assert_eq(p->age, 30);
  
  deallocate_person(p);
  free(p);
}