#include <criterion/criterion.h>
#include <stdlib.h>
#include "link.h"

Test(link, add_and_remove_nodes) {
  LinkedList *list = getLinkedListInstance();
  
  int *data1 = malloc(sizeof(int));
  *data1 = 42;
  addNode(list, data1);
  
  int *data2 = malloc(sizeof(int));
  *data2 = 84;
  addNode(list, data2);
  
  Data removedData1 = removeNode(list);
  cr_assert_eq(*(int*)removedData1, 84);
  
  Data removedData2 = removeNode(list);
  cr_assert_eq(*(int*)removedData2, 42);
  
  Data removedData3 = removeNode(list);
  cr_assert_null(removedData3);
  
  free(data1);
  free(data2);
  removeLinkedListInstance(list);
}