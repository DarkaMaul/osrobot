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

    /* Game */
    unsigned char type;
    unsigned char role;
    unsigned char side;
    unsigned char ally;

} state;

typedef struct _mainpositions
{
    position s_fr_init;
    position s_fr_ballarea;
    position s_fr_ending;

    position l_fr_init;
    position l_fr_dodgefirst;
    position l_fr_ballarea;
    position l_fr_dodgesecond;
    position l_fr_ending;

    position s_sr_init;
    position s_sr_ballarea;
    position s_sr_ending;

    position l_sr_init;
    position l_sr_dodgefirst;
    position l_sr_ballarea;
    position l_sr_dodgesecond;
    position l_sr_ending;
} mainpos;



#endif
