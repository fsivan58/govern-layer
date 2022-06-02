// std includes
#include <stdio.h>
#include <stdlib.h>

// kernel
// #include "AuK_v1_1_7.h"

// custom includes
#include "defs.h"
#include "msgQ.h"
#include "instruction_handler.h"
#include "uart_handler.h"

void action(ushort msg);
void read_sensor();
void set_coordinates();
void set_parameters();
void set_feedback();
int get_sensor();
void get_checkpoint(checkpoint_t *cp);
void get_parameters(parameters_t *params);
void wait_for_q(message_queue_t *q, int period, int *retries);

/**
 * @brief Main part of the instruction handler task. Its only function is to
 * wait for the Govern to receive a message either from the Operator or the
 * Navigation layer
 *
 */
void instr_handler()
{
    int period = 10;
    int awake;
    ushort msg = NULL;

    FOREVER
    {
        awake = clock() + period;
        if (!isEmpty(&rcvq))
        {
            msg = readMsg(&rcvq);
            action(msg);
        }
        delay_until(awake);
    }
}

/**
 * @brief Function used to decide what to do with the first byte of the
 * instruction, which indicates the requested operation type
 *
 * @param msg Byte received from the Operator or Navigator layer
 */
void action(ushort msg)
{
    switch (msg)
    {
    case 0x00:
        read_sensor();
        break;

    case 0x01:
        set_coordinates();
        break;

    case 0x02:
        set_parameters();
        break;

    case 0x03:
        set_feedback();
        break;

    default:
        break;
    }
}

/**
 * @brief Calls UART functionality to read a port when the sensor ID is set
 *
 */
void read_sensor()
{
    int awake;
    int period = 10;
    int retries = 10;
    ushort msg;

    wait_for_q(&rcvq, period, &retries);
    msg = readMsg(&rcvq);
    wait(&m_sensor);
    sensor.id = msg;
    sensor.set = 1;
    signal(&m_sensor);
}

/**
 * @brief Set the coordinates object
 *
 */
void set_coordinates()
{
    wait(&m_checkpoint);
    get_checkpoint(&cp);
    cp.set = 1;
    signal(&m_checkpoint);
}

/**
 * @brief Set the parameters object
 *
 */
void set_parameters()
{
    wait(&m_params);
    get_parameters(&params);    
    params.set = 1;
    signal(&m_params);
}

/**
 * @brief Set the feedback object
 *
 */
void set_feedback()
{
    ushort msg;
    int period = 10;
    int retries = 10;

    // TODO change this variables for global variables
    int sensor;
    checkpoint_t cp;

    wait_for_q(&rcvq, period, &retries);
    msg = readMsg(&rcvq);

    switch (msg)
    {
    case 0x00:
        sensor = get_sensor();
        break;

    case 0x01:
        get_checkpoint(&cp);
        break;

    default:
        break;
    }
}

/**
 * @brief Get the sensor object
 *
 * @return int
 */
int get_sensor()
{
    int awake;
    int period = 10;
    int retries = 10;
    int id, value, set;
    ushort msg;

    wait_for_q(&rcvq, period, &retries);
    msg = readMsg(&rcvq);
    id = msg;

    wait_for_q(&rcvq, period, &retries);
    msg = readMsg(&rcvq);
    value = msg;
    set = 2;

    wait(&m_sensor);
    sensor.id = id;
    sensor.value = value;
    sensor.set = set;
    signal(&m_sensor);
}

/**
 * @brief Get the checkpoint object
 *
 * @param cp
 */
void get_checkpoint(checkpoint_t *cp)
{
    int period = 10;
    int retries = 10;
    ushort axis_x_low, axis_x_high;
    ushort axis_y_low, axis_y_high;

    wait_for_q(&rcvq, period, &retries);
    axis_x_low = readMsg(&rcvq);
    retries = 10;

    wait_for_q(&rcvq, period, &retries);
    axis_x_high = readMsg(&rcvq);
    retries = 10;

    wait_for_q(&rcvq, period, &retries);
    axis_y_low = readMsg(&rcvq);
    retries = 10;

    wait_for_q(&rcvq, period, &retries);
    axis_y_high = readMsg(&rcvq);

    wait(&m_checkpoint);
    cp->axis_x = axis_x_high;
    cp->axis_x = cp->axis_x << 8;
    cp->axis_x |= axis_x_low;

    cp->axis_y = axis_y_high;
    cp->axis_y = cp->axis_y << 8;
    cp->axis_y |= axis_y_low;

    cp->set = 2;
    signal(&m_checkpoint);
}

/**
 * @brief Get the params object
 *
 * @param params
 */
void get_parameters(parameters_t *params)
{
    int period = 10;
    int retries = 10;

    wait_for_q(&rcvq, period, &retries);
    params->angular_speed = readMsg(&rcvq);

    retries = 10;

    wait_for_q(&rcvq, period, &retries);
    params->linear_speed = readMsg(&rcvq);

    retries = 10;

    wait_for_q(&rcvq, period, &retries);
    params->time = readMsg(&rcvq);
}

/**
 * @brief Loop used for wait for the queue to receive messages. If retries
 * goes to 0, an error/warning will be raised
 *
 * @param q Reference to the queue which the program is waiting for
 * @param period Time to delay the task
 * @param retries Remain attempts to read the queue
 */
void wait_for_q(message_queue_t *q, int period, int *retries)
{
    int awake;

    while (isEmpty(q) && *retries)
    {
        awake = clock() + period;
        --(*retries);
        delay_until(awake);
    }

    if (!(*retries))
    {
        exit(-1);
        // TODO change exit for error/warning handling.
        // Decide whether to exit or not
    }
}
