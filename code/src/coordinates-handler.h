#ifndef COORDINATES_HANDLER
#define COORDINATES_HANDLER

#include "global.h"
#include "definitions.h"

extern Queue coordinates_q;
extern Queue q;

void init_coordinates_q(int size);
void handle_coordinates();

#endif /* COORDINATES_HANDLER */