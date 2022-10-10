#ifndef QUEUE
#define QUEUE

#define MAX_Q_SIZE 32

typedef struct {
    int q[MAX_Q_SIZE];
    int size;
} Queue;

void new_q (Queue *q);
int pop (Queue *q);
int add (Queue *q, int element);
int is_empty (Queue *q);
int is_full (Queue *q);
void destroy_q (Queue *q);

#endif