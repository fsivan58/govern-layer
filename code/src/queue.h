#ifndef QUEUE
#define QUEUE

#define MAX_Q_SIZE 32

typedef struct {
    int q[MAX_Q_SIZE];
    int size;
} Queue;

void init_q (Queue *q, int size);
int dequeue (Queue *q);
int enqueue (Queue *q, int element);
int is_empty (Queue *q);
int is_full (Queue *q);
void destroy_q (Queue *q);

#endif