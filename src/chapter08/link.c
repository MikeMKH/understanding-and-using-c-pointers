#include <stdlib.h>
#include "link.h"

typedef struct _node {
  Data data;
  struct _node *next;
} Node;

struct _linkedList {
  Node *head;
};

LinkedList* getLinkedListInstance(void) {
  LinkedList *list = malloc(sizeof(LinkedList));
  list->head = NULL;
  return list;
}

void removeLinkedListInstance(LinkedList *list) {
  Node *current = list->head;
  while (current != NULL) {
    free(current->data); /* Assuming data was dynamically allocated, otherwise this could cause a memory leak */
    Node *temp = current;
    current = current->next;
    free(temp);
  }
  free(list);
}

void addNode(LinkedList *list, Data data) {
  Node *newNode = malloc(sizeof(Node));
  newNode->data = data;
  if (list->head == NULL) {
    newNode->next = NULL;
    list->head = newNode;
  } else {
    newNode->next = list->head;
    list->head = newNode;
  }
}

Data removeNode(LinkedList *list) {
  if (list->head == NULL) {
    /* list is empty */
    return NULL;
  }
  Node *temp = list->head;
  Data data = temp->data;
  list->head = temp->next;
  free(temp);
  return data;
}