#ifndef SENSOR_HANDLER
#define SENSOR_HANDLER

#include "global.h"
#include "definitions.h"

extern Queue q;
extern Queue sensor_q;

extern Tsemaphore sensor_s;

void init_sensor_q(int size);
void handle_sensor(int msg);

#endif /* SENSOR_HANDLER */