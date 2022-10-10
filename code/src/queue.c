#include <queue.h>

void new_q (Queue *q){
    int i = 0;

    q = malloc(sizeof(Queue));
    q->size = 0;
    for (i; i < MAX_Q_SIZE; ++i) q->q[i] = 0;
}

int pop (Queue *q){
    int i = 0;
    int val = q->q[0];

    if (!is_empty(q)) {
        for (i; i < q->size - 1; ++q) {
            q->q[i] = q->q[i+1];
        }
        q->size--;
    } else {
        val = -1;
    }

    return val;
}

int add (Queue *q, int element){
    int i = 0;
    int val = 1;

    if (!is_full(q)) {
        q->q[q->size] = element;
    } else {
        val = -1;
    }

    return val;
}

int is_empty (Queue *q){
    return q->size == 0;
}

int is_full (Queue *q){
    return q->size == MAX_Q_SIZE;
}

void destroy_q (Queue *q){

}
