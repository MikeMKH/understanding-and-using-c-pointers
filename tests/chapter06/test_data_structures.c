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

Test(linked_list, initialize_linked_list) {
  LinkedList* list = initialize_linked_list(NULL);
  cr_assert(list->head == NULL);
  cr_assert(list->tail == NULL);
  cr_assert(list->current == NULL);
  
  initialize_linked_list(list);
  cr_assert(list->head == NULL);
  cr_assert(list->tail == NULL);
  cr_assert(list->current == NULL);
  
  deallocate_linked_list(list);
}

Test(linked_list, add_header_and_add_tail) {
  LinkedList* list = initialize_linked_list(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  
  add_header(list, e1);
  cr_assert(list->head != NULL);
  cr_assert(list->tail == list->head);
  
  add_tail(list, e2);
  cr_assert(list->tail != NULL);
  cr_assert(list->tail != list->head);
  
  deallocate_linked_list(list);
}

Test(linked_list, delete_node) {
  LinkedList* list = initialize_linked_list(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  
  add_header(list, e1);
  add_tail(list, e2);
  
  Node *node_to_delete = get_node(list, (COMPARE)compare_employees, e1);
  delete(list, node_to_delete);
  
  cr_assert(list->head != node_to_delete);
  cr_assert(list->tail != node_to_delete);
  
  Node *node = get_node(list, (COMPARE)compare_employees, e2);
  cr_assert(list->head == node);
  cr_assert(list->tail == node);
  
  deallocate_linked_list(list);
}

Test(linked_list, get_node) {
  LinkedList* list = initialize_linked_list(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Charlie", 35);
  
  add_tail(add_tail(add_header(list, e1), e2), e3);
  
  Node *node3 = get_node(list, (COMPARE)compare_employees, e3);
  Node *node1 = get_node(list, (COMPARE)compare_employees, e1);
  Node *node2 = get_node(list, (COMPARE)compare_employees, e2);
  Node *node4 = get_node(list, (COMPARE)compare_employees, e3);
  
  cr_assert(node1 != NULL);
  cr_assert(node2 != NULL);
  cr_assert(node3 != NULL);
  cr_assert(node4 != NULL);
  
  cr_assert(compare_employees(node1->data, e1) == 0);
  cr_assert(compare_employees(node2->data, e2) == 0);
  cr_assert(compare_employees(node3->data, e3) == 0);
  cr_assert(compare_employees(node4->data, e3) == 0);
  
  deallocate_linked_list(list);
}

Test(linked_list, format_linked_list) {
  LinkedList* list = initialize_linked_list(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  
  add_header(list, e1);
  add_tail(list, e2);
  
  char *formatted = format_linked_list(list, format_employee);
  cr_assert_str_eq(formatted, "Name: Alice, Age: 30 -> Name: Bob, Age: 25");
  
  free(formatted);
  deallocate_linked_list(list);
}