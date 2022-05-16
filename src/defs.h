#ifndef __DEFS__
#define __DEFS__

#define FOREVER for (;;)

// Sensors IDs
#define IR 0x00
#define US 0x01
#define IMU 0x02
#define LIDAR 0x03

typedef struct checkpoint
{
  axis_x;
  axis_y;
} checkpoint_t;

extern int sensor_read;
extern int checkpoint_read;
extern int paremeters_ready;
extern int coordinates_ready;

#endif