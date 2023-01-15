#ifndef QUEUE
#define QUEUE

#define MAX_Q_SIZE 32

typedef struct {
    int q[MAX_Q_SIZE];
    int size;
} Queue;

/*
    Initialization of q with size
*/
void init_q (Queue *q, int size);

/*
    Reads the head of q
    Returns -1 if q is empty
*/
int dequeue (Queue *q);

/*
    Reads the head of q
    Returns -1 if q is full
*/
int enqueue (Queue *q, int element);

/*
    0 - q is empty
    1 - q has elements
*/
int is_empty (Queue *q);

/*
    0 - q is full
    1 - q has remaining space
*/
int is_full (Queue *q);

void destroy_q (Queue *q);

#endif