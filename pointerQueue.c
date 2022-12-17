// Queue ADT implementation ... COMP9024 22T3
// modified to store a generic pointer instead of int

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "pointerQueue.h"

typedef struct node {
    void* data;
    struct node* next;
} NodeT;

typedef struct QueueRep {
    int length;
    NodeT* head;
    NodeT* tail;
} QueueRep;

// set up empty queue
queue newQueue() {
    queue Q = malloc(sizeof(QueueRep));
    assert(Q != NULL);

    Q->length = 0;
    Q->head = NULL;
    Q->tail = NULL;
    return Q;
}

// remove unwanted queue
void dropQueue(queue Q) {
    NodeT* curr = Q->head;

    while (curr != NULL) {
        NodeT* temp = curr->next;
        free(curr);
        curr = temp;
    }

    free(Q);
}

// check whether queue is empty
bool QueueIsEmpty(queue Q) {
    return (Q->length == 0);
}

// insert a generic pointer at end of queue
void QueueEnqueue(queue Q, void* p) {
    NodeT* new = malloc(sizeof(NodeT));
    assert(new != NULL);

    new->data = p;
    new->next = NULL;

    if (Q->tail != NULL) {
        Q->tail->next = new;
        Q->tail = new;
    } else {
        Q->head = Q->tail = new;
    }

    Q->length++;
}

// remove generic pointer from front of queue
void* QueueDequeue(queue Q) {
    assert(Q->length > 0);

    NodeT* p = Q->head;
    Q->head = Q->head->next;

    if (Q->head == NULL)
        Q->tail = NULL;

    Q->length--;

    void* d = p->data;
    free(p);
    return d;
}
