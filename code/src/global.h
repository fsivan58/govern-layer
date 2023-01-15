#ifndef GLOBAL
#define GLOBAL

#include "AuK_v1_1_8.h"
#include "queue.h"

Queue q;
Queue sensor_q;
Queue coordinates_q;
Queue params_q;
Queue feedback_q;

Tsemaphore s;
Tsemaphore sensor_s;

#endif /* GLOBAL */