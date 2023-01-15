#include "sensor-handler.h"

void init_sensor_q(int size) {
    init_q(&sensor_q, size);
}

void handle_sensor(int msg) {
    wait(&sensor_s);
    uint8 sensor = (msg & 0x00FF0000) >> 16;
    enqueue(&sensor_q, sensor);
    signal(&sensor_s);
}
