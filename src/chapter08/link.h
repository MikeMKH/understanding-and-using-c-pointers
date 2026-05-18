typedef void *Data;
typedef struct _linkedList LinkedList;

LinkedList* getLinkedListInstance(void);
void removeLinkedListInstance(LinkedList *list);
void addNode(LinkedList *list, Data data);
Data removeNode(LinkedList *list);