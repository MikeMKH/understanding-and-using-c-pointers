#include "data_structures.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Employee* create_employee(const char *name, unsigned char age) {
  Employee *e = malloc(sizeof(Employee));
  strncpy(e->name, name, sizeof(e->name) - 1);
  e->name[sizeof(e->name) - 1] = '\0'; // Ensure null-termination
  e->age = age;
  return e;
}

int compare_employees(void *e1, void *e2) {
  return strcmp(((const Employee *)e1)->name, ((const Employee *)e2)->name);
}

void format_employee(void *e, char *buffer, size_t buffer_size) {
  snprintf(buffer, buffer_size, "Name: %s, Age: %u", ((const Employee *)e)->name, ((const Employee *)e)->age);
}


LinkedList* initialize_linked_list(LinkedList *list) {
  if (!list) { list = malloc(sizeof(LinkedList)); }
  list->head = list->tail = list->current = NULL;
  return list;
}

void deallocate_node(Node *node) {
  free(node);
}

void deallocate_linked_list(LinkedList *list) {
  Node *current = list->head;
  while (current) {
    Node *next = current->next;
    deallocate_node(current);
    current = next;
  }
  free(list);
}

LinkedList* add_header(LinkedList *list, void *data) {
  Node *node = malloc(sizeof(Node));
  node->data = data;
  node->next = list->head;
  
  list->head = node;
  if (list->tail == NULL) {
    list->tail = node;
  }
  return list;
}

LinkedList* add_tail(LinkedList *list, void *data) {
  Node *node = malloc(sizeof(Node));
  node->data = data;
  node->next = NULL;
  
  if (list->tail) {
    list->tail->next = node;
  }
  list->tail = node;
  
  if (list->head == NULL) {
    list->head = node;
  }
  return list;
}

LinkedList* delete(LinkedList *list, Node *node) {
  if (list->head == NULL || node == NULL) {
    return list;
  }
  
  if (list->head == node) {
    if (list->head->next == NULL) {
      list->head = list->tail = NULL;
    } else {
      list->head = list->head->next;
    }
  } else {
    Node *temp = list->head;
    while (temp != NULL && temp->next != node) {
      temp = temp->next;
    }
    if (temp != NULL) {
      temp->next = node->next;
      if (node == list->tail) {
        list->tail = temp;
      }
    }
  }
  
  deallocate_node(node);
  return list;
}

Node* get_node(LinkedList *list, COMPARE compare, void *data) {
  Node *node = list->head;
  while (node) {
    if (compare(node->data, data) == 0) {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

char* format_linked_list(LinkedList *list, FORMAT format) {
  size_t buffer_size = 256;
  char *buffer = malloc(buffer_size);
  buffer[0] = '\0';
  
  Node *node = list->head;
  while (node) {
    char item_buffer[128];
    format(node->data, item_buffer, sizeof(item_buffer));
    strncat(buffer, item_buffer, buffer_size - strlen(buffer) - 1);
    if (node->next) {
      strncat(buffer, " -> ", buffer_size - strlen(buffer) - 1);
    }
    node = node->next;
  }
  
  return buffer;
}

Queue* initialize_queue(Queue *queue) {
  return initialize_linked_list(queue);
}

void deallocate_queue(Queue *queue) {
  deallocate_linked_list(queue);
}

Queue* enqueue(Queue *queue, void *data) {
  return add_header(queue, data);
}

void* dequeue(Queue *queue) {
  if (queue->tail == NULL) {
    return NULL;
  }
  
  void *data = queue->tail->data;
  delete(queue, queue->tail);
  return data;
}

char* format_queue(Queue *queue, FORMAT format) {
  return format_linked_list(queue, format);
}

Stack* initialize_stack(Stack *stack) {
  return initialize_linked_list(stack);
}

void deallocate_stack(Stack *stack) {
  deallocate_linked_list(stack);
}

Stack* push(Stack *stack, void *data) {
  return add_header(stack, data);
}

void* pop(Stack *stack) {
  if (stack->head == NULL) {
    return NULL;
  }
  
  Node *node = stack->head;
  void *data = node->data;
  if (stack->head->next == NULL) {
    stack->head = stack->tail = NULL;
  } else {
    stack->head = stack->head->next;
  }
  deallocate_node(node);
  return data;
}

char* format_stack(Stack *stack, FORMAT format) {
  return format_linked_list(stack, format);
}

void insert_node(TreeNode **realRoot, COMPARE compare, void *data) {
  TreeNode *node = malloc(sizeof(TreeNode));
  node->data = data;
  node->left = node->right = NULL;
  
  TreeNode *root = *realRoot;
  if (root == NULL) {
    *realRoot = node;
    return;
  }
  
  while (1) {
    if (compare((root)->data, data) > 0) {
      if ((root)->left != NULL) {
        root = (root)->left;
      } else {
        (root)->left = node;
        break;
      }
    } else {
      if ((root)->right != NULL) {
        root = (root)->right;
      } else {
        (root)->right = node;
        break;
      }
    }
  }
  return;
}

void append_formatted_item(TreeNode *node, FORMAT format, char *buffer, size_t buffer_size) {
  char item_buffer[128];
  format(node->data, item_buffer, sizeof(item_buffer));
  strncat(buffer, item_buffer, buffer_size - strlen(buffer) - 1);
  strncat(buffer, " -> ", buffer_size - strlen(buffer) - 1);
}

void trim_trailing_separator(char *buffer) {
  size_t len = strlen(buffer);
  if (len >= 4 && strcmp(buffer + len - 4, " -> ") == 0) {
    buffer[len - 4] = '\0';
  }
}

void in_order_traversal_impl(TreeNode *node, FORMAT format, char *buffer, size_t buffer_size) {
  if (node == NULL) {
    return;
  }
  
  in_order_traversal_impl(node->left, format, buffer, buffer_size);
  append_formatted_item(node, format, buffer, buffer_size);
  in_order_traversal_impl(node->right, format, buffer, buffer_size);
}

void in_order_traversal(TreeNode *node, FORMAT format, char *buffer, size_t buffer_size) {
  in_order_traversal_impl(node, format, buffer, buffer_size);
  trim_trailing_separator(buffer);
}

void post_order_traversal_impl(TreeNode *node, FORMAT format, char *buffer, size_t buffer_size) {
  if (node == NULL) {
    return;
  }
  
  post_order_traversal_impl(node->left, format, buffer, buffer_size);
  post_order_traversal_impl(node->right, format, buffer, buffer_size);
  append_formatted_item(node, format, buffer, buffer_size);
}

void post_order_traversal(TreeNode *node, FORMAT format, char *buffer, size_t buffer_size) {
  post_order_traversal_impl(node, format, buffer, buffer_size);
  trim_trailing_separator(buffer);
}

void pre_order_traversal_impl(TreeNode *node, FORMAT format, char *buffer, size_t buffer_size) {
  if (node == NULL) {
    return;
  }
  
  append_formatted_item(node, format, buffer, buffer_size);
  pre_order_traversal_impl(node->left, format, buffer, buffer_size);
  pre_order_traversal_impl(node->right, format, buffer, buffer_size);
}

void pre_order_traversal(TreeNode *node, FORMAT format, char *buffer, size_t buffer_size) {
  pre_order_traversal_impl(node, format, buffer, buffer_size);
  trim_trailing_separator(buffer);
}