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
    position ballPosition;
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

    int gyro_reference;
    int compass_reference;

    uint16_t msgId;
	uint16_t msgAck;

    //Threads and concurrent access
    pthread_t threadPosition;
    pthread_mutex_t mutexPosition;
    pthread_mutex_t mutexGameStarted;
    pthread_mutex_t mutexSockUsage;

    /* Game */
    int gameStarted;
    unsigned char type;
    unsigned char role;
    unsigned char side;
    unsigned char ally;

} state;


void game();

#endif
