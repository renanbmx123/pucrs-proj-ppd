#include "stdlib.h"
#include "list.h"
#include <string.h>

void list_push(List *l, void *data, size_t data_size){
    Node *node = malloc(sizeof(Node));
    node->next = NULL;
    node->value = malloc(data_size);
    memcpy(node->value, data, data_size);
    if(l->head == NULL){
        l->head = node;
    }
    else{
        Node *aux = l->tail;
        aux->next = node;
    }
    l->tail = node;
}

void *list_get(List *l, int index){
    int i = 0; 
    Node *n = l->head;
    while(n != NULL){
        if(i++ == index){
            return n->value;
        }
        n = n->next;
    }
    return NULL;
}

int list_set(List *l, int index, void *data, size_t data_size){
    int result = 0, i = 0; 
    Node *n = l->head;
    while(n != NULL){
        if(i++ == index){
            free(n->value);
            n->value = malloc(data_size);
            memcpy(n->value, data, data_size);
            result = 1;
            break;
        }
        n = n->next;
        i++;
    }
    return result;
}

int list_size(List *l){
    int i = 0; 
    Node *n = l->head;
    while(n != NULL){
        n = n->next;
        i++;
    }
    return i;
}
