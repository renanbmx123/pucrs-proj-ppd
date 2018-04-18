#ifndef _LISTA
#define _LISTA

typedef struct Node{
    void *value;
    struct Node *next;
} Node;

typedef struct {
    Node *head, *tail;
} List;

void list_push(List *l, void *data, size_t data_size);
void *list_get(List *l, int index);
int list_set(List *l, int index, void *data, size_t data_size);
int list_size(List *l);

#endif
