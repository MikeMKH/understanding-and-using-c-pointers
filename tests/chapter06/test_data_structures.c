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
  free(e1);
  free(e2);
}

Test(linked_list, add_header_multiple) {
  LinkedList* list = initialize_linked_list(NULL);
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Clare", 35);
  
  add_header(list, e1);
  add_header(list, e2);
  add_header(list, e3);
  
  cr_assert(list->head->data == e3);
  cr_assert(list->head->next->data == e2);
  cr_assert(list->tail->data == e1);
  
  deallocate_linked_list(list);
  free(e1);
  free(e2);
  free(e3);
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
  
  free(e1);
  deallocate_linked_list(list);
  free(e2);
}

Test(linked_list, get_node) {
  LinkedList* list = initialize_linked_list(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Clare", 35);
  
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
  free(e1);
  free(e2);
  free(e3);
}

Test(linked_list, get_node_not_found) {
  LinkedList* list = initialize_linked_list(NULL);
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Clare", 35);
  
  add_tail(list, e1);
  add_tail(list, e2);
  
  Node *node = get_node(list, compare_employees, e3);
  cr_assert(node == NULL);
  
  deallocate_linked_list(list);
  free(e3);
}

Test(linked_list, get_node_empty_list) {
  LinkedList* list = initialize_linked_list(NULL);
  Employee *e1 = create_employee("Alice", 30);
  
  Node *node = get_node(list, compare_employees, e1);
  cr_assert(node == NULL);
  
  free(list);
  free(e1);
}

Test(linked_list, multiple_nodes_order) {
  LinkedList* list = initialize_linked_list(NULL);
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Clare", 35);
  
  add_tail(list, e1);
  add_tail(list, e2);
  add_tail(list, e3);
  
  cr_assert(list->head->data == e1);
  cr_assert(list->head->next->data == e2);
  cr_assert(list->tail->data == e3);
  
  deallocate_linked_list(list);
  free(e1);
  free(e2);
  free(e3);
}

Test(linked_list, delete_tail_node) {
  LinkedList* list = initialize_linked_list(NULL);
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  
  add_tail(list, e1);
  add_tail(list, e2);
  
  Node *tail_node = list->tail;
  delete(list, tail_node);
  
  cr_assert(list->tail == list->head);
  cr_assert(list->head->data == e1);
  
  free(e2);
  deallocate_linked_list(list);
  free(e1);
}

Test(linked_list, delete_middle_node) {
  LinkedList* list = initialize_linked_list(NULL);
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Clare", 35);
  
  add_tail(list, e1);
  add_tail(list, e2);
  add_tail(list, e3);
  
  Node *middle = list->head->next;
  delete(list, middle);
  
  cr_assert(list->head->data == e1);
  cr_assert(list->head->next->data == e3);
  cr_assert(list->tail->data == e3);
  
  free(e2);
  deallocate_linked_list(list);
  free(e1);
  free(e3);
}

Test(linked_list, delete_single_node) {
  LinkedList* list = initialize_linked_list(NULL);
  Employee *e1 = create_employee("Alice", 30);
  
  add_tail(list, e1);
  
  Node *only_node = list->head;
  delete(list, only_node);
  
  cr_assert(list->head == NULL);
  cr_assert(list->tail == NULL);
  
  free(list);
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
  free(e1);
  free(e2);
}

Test(queue, initialize_queue) {
  Queue* queue = initialize_queue(NULL);
  cr_assert(queue->head == NULL);
  cr_assert(queue->tail == NULL);
  cr_assert(queue->current == NULL);
  
  initialize_queue(queue);
  cr_assert(queue->head == NULL);
  cr_assert(queue->tail == NULL);
  cr_assert(queue->current == NULL);
  
  deallocate_queue(queue);
}

Test(queue, enqueue_and_dequeue) {
  Queue* queue = initialize_queue(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Clare", 35);
  
  enqueue(queue, e1);
  enqueue(queue, e2);
  enqueue(queue, e3);
  
  void *dequeued1 = dequeue(queue);
  void *dequeued2 = dequeue(queue);
  void *dequeued3 = dequeue(queue);
  
  cr_assert(dequeued1 == e1);
  cr_assert(dequeued2 == e2);
  cr_assert(dequeued3 == e3);
  
  queue = enqueue(enqueue(queue, e2), e1);
  void *dequeued4 = dequeue(queue);
  cr_assert(dequeued4 == e2);
  void *dequeued5 = dequeue(queue);
  cr_assert(dequeued5 == e1);
  
  deallocate_queue(queue);
  free(e1);
  free(e2);
  free(e3);
}

Test(queue, dequeue_empty_queue) {
  Queue* queue = initialize_queue(NULL);
  
  void *dequeued = dequeue(queue);
  cr_assert(dequeued == NULL);
  
  deallocate_queue(queue);
}

Test(queue, format_queue) {
  Queue* queue = initialize_queue(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  
  enqueue(queue, e1);
  enqueue(queue, e2);
  
  char *formatted = format_queue(queue, format_employee);
  cr_assert_str_eq(formatted, "Name: Bob, Age: 25 -> Name: Alice, Age: 30");
  
  free(formatted);
  deallocate_queue(queue);
  free(e1);
  free(e2);
}

Test(stack, initialize_stack) {
  Stack* stack = initialize_stack(NULL);
  cr_assert(stack->head == NULL);
  cr_assert(stack->tail == NULL);
  cr_assert(stack->current == NULL);
  
  initialize_stack(stack);
  cr_assert(stack->head == NULL);
  cr_assert(stack->tail == NULL);
  cr_assert(stack->current == NULL);
  
  deallocate_stack(stack);
}

Test(stack, push_and_pop) {
  Stack* stack = initialize_stack(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Clare", 35);
  
  push(stack, e1);
  push(stack, e2);
  push(stack, e3);
  
  void *popped1 = pop(stack);
  void *popped2 = pop(stack);
  void *popped3 = pop(stack);
  
  cr_assert(popped1 == e3);
  cr_assert(popped2 == e2);
  cr_assert(popped3 == e1);
  
  push(push(stack, e2), e1);
  void *popped4 = pop(stack);
  cr_assert(popped4 == e1);
  void *popped5 = pop(stack);
  cr_assert(popped5 == e2);
  
  deallocate_stack(stack);
  free(e1);
  free(e2);
  free(e3);
}

Test(stack, pop_until_empty) {
  Stack* stack = initialize_stack(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  Employee *e3 = create_employee("Alice", 30);
  
  push(push(push(stack, e1), e2), e3);
  
  void *popped1 = pop(stack);
  void *popped2 = pop(stack);
  void *popped3 = pop(stack);
  void *popped4 = pop(stack);
  
  cr_assert(popped1 == e3);
  cr_assert(popped2 == e2);
  cr_assert(popped3 == e1);
  cr_assert(popped4 == NULL);
  
  deallocate_stack(stack);
  free(e1);
  free(e2);
  free(e3);
}

Test(stack, pop_single_element) {
  Stack* stack = initialize_stack(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  push(stack, e1);
  
  void *popped = pop(stack);
  cr_assert(popped == e1);
  
  void *popped_again = pop(stack);
  cr_assert(popped_again == NULL);
  
  deallocate_stack(stack);
  free(e1);
}

Test(stack, pop_empty_stack) {
  Stack* stack = initialize_stack(NULL);
  
  void *popped = pop(stack);
  cr_assert(popped == NULL);
  
  deallocate_stack(stack);
}

Test(stack, format_stack) {
  Stack* stack = initialize_stack(NULL);
  
  Employee *e1 = create_employee("Alice", 30);
  Employee *e2 = create_employee("Bob", 25);
  
  push(stack, e1);
  push(stack, e2);
  
  char *formatted = format_stack(stack, format_employee);
  cr_assert_str_eq(formatted, "Name: Bob, Age: 25 -> Name: Alice, Age: 30");
  
  free(formatted);
  deallocate_stack(stack);
  free(e1);
  free(e2);
}