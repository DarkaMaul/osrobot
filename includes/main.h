#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "ev3c.h"

typedef struct _position
{
    int x;
    int y;
} position;

typedef struct _state
{
	//External ressources needed
	//Log file
	FILE * logfile_fd;

    //Socket
    int sock;

	/* Position of the robot */
    position curPos;
    position wantedPos;
	int pos[2];
	int angle;

	/* Motors */
	ev3_motor_ptr motors;
	ev3_motor_ptr leftmotor;
	ev3_motor_ptr rightmotor;
	ev3_motor_ptr grabmotor;
	ev3_motor_ptr sweepmotor;

    //Sensors
    ev3_sensor_ptr sensors;
    ev3_sensor_ptr color;
    ev3_sensor_ptr gyro;
    ev3_sensor_ptr sonar;
    ev3_sensor_ptr compass;

    uint16_t msgId;
	uint16_t msgAck;

    /* Game */
    int role;
    int side;
    int ally;

} state;

#endif
