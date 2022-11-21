// std libraries
#include <stdlib.h>

// custom libraries 
#include "queue.h"

void init_q (Queue *q, int size)
{
    int i;

    q->size = 0;
    for (i = 0; i < size; ++i)
    {
        q->q[i] = 0;
    }
}

int dequeue (Queue *q)
{
    int i;
    int val = q->q[0];

    if (!is_empty(q))
    {
        for (i = 0; i < q->size - 1; ++q)
        {
            q->q[i] = q->q[i+1];
        }
        q->size--;
    }
    else
    {
        val = -1;
    }

    return val;
}

int enqueue (Queue *q, int element)
{
    int val = 1;

    if (!is_full(q)) {
        q->q[q->size] = element;
        ++q->size;
    } else {
        val = -1;
    }

    return val;
}

int is_empty (Queue *q)
{
    return q->size == 0;
}

int is_full (Queue *q)
{
    return q->size == MAX_Q_SIZE;
}

void destroy_q (Queue *q)
{

}
