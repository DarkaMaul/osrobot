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
		ev3_set_position(motor, 0);
		log_this(s, "Motor %i on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
		motor = motor->next;
	}
	ev3_stop_command_motor_by_name(s->grabmotor, "hold");
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
/*
 * TODO Function that will be used to go to a specified position
 *
 */
int go_to_pos(state *s,int *desiredposition){
	while((s->pos[0]!=desiredposition[0])&&(s->pos[1]!=desiredposition[1])){

	}
	return 0;
}
/**
 * Go straightforward at a specified distance with specified speed
 */
int go_straight(state *s,int speed, int distance){
	ev3_set_speed_sp(s->leftmotor, speed);
	ev3_set_speed_sp(s->rightmotor, speed);
	ev3_set_position_sp(s->grabmotor, distance);
	ev3_command_motor_by_name(s->grabmotor, "run-to-abs-pos");
	while (ev3_motor_state(s->grabmotor) & MOTOR_RUNNING);
	return 0;
}
