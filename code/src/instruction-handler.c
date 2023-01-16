#include <stdio.h>

#include "AuK_v1_1_8.h"

#include "global.h"
#include "definitions.h"
#include "queue.h"
#include "instruction-handler.h"
#include "sensor-handler.h"
#include "coordinates-handler.h"
#include "parameters-handler.h"
#include "feedback-handler.h"

extern Queue q;
extern Queue sensor_q;
extern Queue coordinates_q;
extern Tsemaphore s;

void instruction_handler()
{
    ulong clk = 1;
    int msg;
    uint8 c;

    while (TRUE)
    {
        clk = clock() + 1000;

        if (!is_empty(&q)) {
            msg = dequeue(&q);
            c = (msg & 0xFF000000) >> 24;

            wait(&s);
            switch(c)
            {
                case 0x01:
                    handle_sensor(msg);
                    break;
                case 0x02:
                    handle_coordinates();
                    break;
                case 0x03:
                    handle_parameters();
                    break;
                case 0x04:
                    handle_feedback();
                    break;
            }
            signal(&s);
        }

        delay_until(clk);
    }

}