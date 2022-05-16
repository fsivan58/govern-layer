#ifndef MSGQ
#define MSGQ

#define MAX_Q_SIZE 10

typedef unsigned int uint;
typedef unsigned short ushort;

typedef struct
{
  ushort q[MAX_Q_SIZE];
  uint counter;
} message_queue_t;

extern message_queue_t rcvq;

int isFull(message_queue_t *q);
void printQ(message_queue_t *q);
int isEmpty(message_queue_t *q);
void addMsg(ushort msg, message_queue_t *q);
ushort readMsg(message_queue_t *q);

#endif