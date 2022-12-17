// Queue ADT header file ... COMP9024 22T3
// modified to store a generic pointer instead of int
#include <stdbool.h>

typedef struct QueueRep* queue;

queue newQueue();                   // set up empty queue
void dropQueue(queue);              // remove unwanted queue
bool QueueIsEmpty(queue);           // check whether queue is empty
void QueueEnqueue(queue, void*);    // insert an generic pointer at end of queue
void* QueueDequeue(queue);          // remove generic pointer from front of queue
