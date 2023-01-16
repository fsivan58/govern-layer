#include "coordinates-handler.h"

void init_coordinates_q(int size) {
    init_q(&coordinates_q, size);
}

void handle_coordinates() {
    int end = !is_empty(&q);
    uint8 flag;
    int msg;

    while(!end) {
        msg = dequeue(&q);
        flag = (msg & 0x00FF0000) >> 16;
        if (flag == 0xFF)
        {
            end = 1;
        }
        else
        {
            // nav <- msg
        }
        end = is_empty(&q);
    }
}