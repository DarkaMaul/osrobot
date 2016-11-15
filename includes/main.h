#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "ev3c.h"

typedef struct _state
{
	//External ressources needed
	//Log file
	FILE * logfile_fd;

    //Socket
    int sock;

	/* Position of the robot */
	int pos[2];
	int angle;

	/* Motors */
	ev3_motor_ptr motors;
	ev3_motor_ptr leftmotor;
	ev3_motor_ptr rightmotor;
	ev3_motor_ptr grabmotor;
	ev3_motor_ptr sweepmotor;

    uint16_t msgId;
    uint16_t msgAck;

    /* Game */
    int role = -1;
    int side = -1;
    int ally = -1;

} state;

void nice_exit(state *s, int exitState);

#endif
