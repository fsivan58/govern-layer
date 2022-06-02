#include <stdio.h>
#include <stdlib.h>

#include "msgQ.h"
#include "defs.h"
#include "uk/AuK_v1_1_7.h"

int main(int argc, char **argv)
{

  char op;
  ushort msg;

  // initialization section
  init_semaphore(&m_checkpoint, 1);

  while (1)
  {
    printf("operation (r/w): ");
    scanf(" %c", &op);
    switch (op)
    {
    case 'w':
      printf("msg: ");
      scanf(" %hu", &msg);
      addMsg(&rcvq, msg);
      break;

    case 'r':
      ushort read = readMsg(&rcvq);
      printf("read %x\n", read);
      break;

    default:
      break;
    }
    printQ(&rcvq);
  }

  return 0;
}