#ifndef __DEFS__
#define __DEFS__

#define FOREVER for (;;)

// Sensors IDs
#define IR 0x00
#define US 0x01
#define IMU 0x02
#define LIDAR 0x03

#include "uk/AuK_v1_1_7.h"

// semaphores

Tmutex m_checkpoint;
Tmutex m_params;
Tmutex m_sensor;

// global structs
/*
  set variable:
    - 0: idle
    - 1: request
    - 2: response
*/

typedef struct checkpoint
{
  int set;
  int axis_x;
  int axis_y;
} checkpoint_t;

extern checkpoint_t cp;

typedef struct parameters
{
  int set;
  int angular_speed;
  int linear_speed;
  int time;
} parameters_t;

extern parameters_t params;

typedef struct sensor
{
  int set;
  int id;
  int value;
} sensor_t;

extern sensor_t sensor;

typedef struct sensor_fb
{
  int set;
  int sensor_id;
  int sensor_status;
} sensorfb_t;

extern sensorfb_t sensorfb;

// global variables

extern int sensor_read;
extern int checkpoint_read;
extern int paremeters_ready;
extern int coordinates_ready;

#endif