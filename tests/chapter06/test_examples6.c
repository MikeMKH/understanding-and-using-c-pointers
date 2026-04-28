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

void reset_person(Person *p) {
    if (!p) return;
    if (p->firstName) free(p->firstName);
    if (p->lastName) free(p->lastName);
    if (p->title) free(p->title);
    p->firstName = p->lastName = p->title = NULL;
    p->age = 0;
}

Person* initialized_person(Person *p,
  const char* firstName, const char* lastName, const char* title, unsigned int age) {
  if (!p) { p = calloc(1, sizeof(Person)); }
  
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

#define PERSON_POOL_SIZE 10
Person *person_pool[PERSON_POOL_SIZE];

void initialize_person_pool(void) {
  for (int i = 0; i < PERSON_POOL_SIZE; i++) {
    person_pool[i] = NULL;
  }
}

Person* get_person(void) {
  for (int i = 0; i < PERSON_POOL_SIZE; i++) {
    if (person_pool[i] != NULL) {
      Person *p = person_pool[i];
      reset_person(p);
      person_pool[i] = NULL; // mark as used
      return p;
    }
  }
  return calloc(1, sizeof(Person));
}

Person* return_person(Person *p) {
  for (int i = 0; i < PERSON_POOL_SIZE; i++) {
    if (person_pool[i] == NULL) {
      reset_person(p);
      person_pool[i] = p;
      return p;
    }
  }
  // pool is full, deallocate and free
  deallocate_person(p);
  free(p);
  return NULL;
}

void setup_person_pool(void) {
  initialize_person_pool();
}

void teardown_person_pool(void) {
  for (int i = 0; i < PERSON_POOL_SIZE; i++) {
    if (person_pool[i] != NULL) {
      deallocate_person(person_pool[i]);
      free(person_pool[i]);
      person_pool[i] = NULL;
    }
  }
}

Test(person_pool, get_person_and_return_person, .init = setup_person_pool, .fini = teardown_person_pool) {
  Person *p1 = get_person();
  initialized_person(p1, "John", "Doe", "Mr.", 30);
  
  cr_assert_str_eq(p1->firstName, "John");
  cr_assert_str_eq(p1->lastName, "Doe");
  cr_assert_str_eq(p1->title, "Mr.");
  cr_assert_eq(p1->age, 30);
  
  return_person(p1);
}

Test(person_pool, get_person_pool_exhaustion, .init = setup_person_pool, .fini = teardown_person_pool) {
  size_t number_of_people = PERSON_POOL_SIZE + 1;
  Person *people[number_of_people];
  for (size_t i = 0; i < number_of_people; i++) {
    people[i] = get_person();
    initialized_person(people[i], "Jane", "Doe", "Mrs.", 30+i);
    
    cr_assert_str_eq(people[i]->firstName, "Jane");
    cr_assert_str_eq(people[i]->lastName, "Doe");
    cr_assert_str_eq(people[i]->title, "Mrs.");
    cr_assert_eq(people[i]->age, 30u+i);
  }
  
  size_t i;
  for (i = 0; i < number_of_people - 1; i++) {
    cr_assert_not_null(return_person(people[i]));
  }
  cr_assert_null(return_person(people[i]));
}
