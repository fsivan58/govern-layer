#include <stdio.h>

#include "msgQ.h"

message_queue_t rcvq = {
    .q = {},
    .counter = 0,
};

message_queue_t sendq = {
    .q = {},
    .counter = 0,
};

void printQ(message_queue_t *q)
{
  int i;
  if (isEmpty(q))
  {
    printf("Queue is empty\n");
  }
  else
  {
    for (i = 0; i < q->counter; ++i)
    {
      if (i == q->counter - 1)
      {
        printf("%x\n", q->q[i]);
      }
      else
      {
        printf("%x, ", q->q[i]);
      }
    }
  }
}

int isFull(message_queue_t *q)
{
  return q->counter == MAX_Q_SIZE;
}

int isEmpty(message_queue_t *q)
{
  return q->counter == 0;
}

void addMsg(ushort msg, message_queue_t *q)
{
  if (!isFull(q))
  {
    q->q[q->counter] = msg;
    ++q->counter;
  }
  else
  {
    printf("Insert error. Queue is full.\n");
  }
}

ushort readMsg(message_queue_t *q)
{
  ushort head = q->q[0];
  int i = 0;

  if (isEmpty(q))
  {
    printf("[FILE %s, FUNCTION %s, LINE %d]: Queue is already empty.\n",
           __FILE__, __FUNCTION__, __LINE__);
    return 0x55;
  }

  for (i = 0; i <= q->counter; ++i)
  {
    if (i <= MAX_Q_SIZE - 2)
    {
      q->q[i] = q->q[i + 1];
    }
  }

  --q->counter;

  return head;
}