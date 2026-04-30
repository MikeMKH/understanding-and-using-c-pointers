#pragma once
#include <stddef.h>

typedef struct _employee {
  char name[32];
  unsigned char age;
} Employee;

Employee* create_employee(const char *name, unsigned char age);
int compare_employees(void *e1, void *e2);
void format_employee(void *e, char *buffer, size_t buffer_size);

typedef int (*COMPARE)(void *e1, void *e2);
typedef void (*FORMAT)(void *e, char *buffer, size_t buffer_size);

typedef struct _node {
  void *data;
  struct _node *next;
} Node;

typedef struct _linked_list {
  Node *head;
  Node *tail;
  Node *current;
} LinkedList;

LinkedList* initialize_linked_list(LinkedList *list);
void deallocate_node(Node *node);
void deallocate_linked_list(LinkedList *list);
LinkedList* add_header(LinkedList *list, void *data);
LinkedList* add_tail(LinkedList *list, void *data);
LinkedList* delete(LinkedList *list, Node *node);
Node* get_node(LinkedList *list, COMPARE compare, void *data);
char* format_linked_list(LinkedList *list, FORMAT format);

typedef LinkedList Queue;
Queue* initialize_queue(Queue *queue);
void deallocate_queue(Queue *queue);
Queue* enqueue(Queue *queue, void *data);
void* dequeue(Queue *queue);
char* format_queue(Queue *queue, FORMAT format);