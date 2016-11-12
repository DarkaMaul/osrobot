/*
 * init.c
 *
 *  Created on: 11 nov. 2016
 *      Author: axel
 */
#include "motors.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"
#include "config.h"

void init_motors(state *s){
	s->motors = ev3_load_motors();
	s->leftmotor=ev3_search_motor_by_port(s->motors, *PORT_LEFT_MOTOR);
	s->rightmotor=ev3_search_motor_by_port(s->motors, *PORT_RIGHT_MOTOR);
	s->grabmotor=ev3_search_motor_by_port(s->motors, *PORT_GRAB_MOTOR);
	s->sweepmotor=ev3_search_motor_by_port(s->motors, *PORT_SWEEP_MOTOR);
	ev3_motor_ptr motor = s->motors;
	//Just in case reset all motors
	while (motor)
	{
		ev3_open_motor(motor);
		ev3_reset_motor(motor);
		log_this(s, "Motor %i on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
		motor = motor->next;
	}
}

/**
 * Function which can be used to grab the ball at a specified speed
 */
int grab(state *s, int speed)
{
	if (ev3_get_position(s->grabmotor) == GRAB_POSITION){
		log_this(s, "Grabbing failed already in grabbing position\n");
		return -1;
	}
	ev3_set_speed_sp(s->grabmotor, speed);
	ev3_set_position_sp(s->grabmotor, GRAB_POSITION);
	ev3_command_motor_by_name(s->grabmotor, "run-to-abs-pos");

	while (ev3_motor_state(s->grabmotor) & MOTOR_RUNNING);
	//log_this(s, "Motor %i on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
	return 0;
}


/**
 * Function which can be used to grab the ball at a specified speed
 */
int release(state *s, int speed)
{
	if (ev3_get_position(s->grabmotor) == INIT_GRAB_POSITION){
		log_this(s, "Releasing failed already in grabbing position\n");
		return -1;
	}
	ev3_set_speed_sp(s->grabmotor, speed);
	ev3_set_position_sp(s->grabmotor, INIT_GRAB_POSITION);
	ev3_command_motor_by_name(s->grabmotor, "run-to-abs-pos");

	while (ev3_motor_state(s->grabmotor) & MOTOR_RUNNING);
	//log_this(s, "Motor %i on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
	return 0;
}
