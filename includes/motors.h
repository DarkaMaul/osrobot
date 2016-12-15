/*
 * init.h
 *
 *  Created on: 11 nov. 2016
 *      Author: axel
 */

#ifndef INCLUDES_MOTORS_H_
#define INCLUDES_MOTORS_H_

#include "main.h"

void init_motors(state * s);
int grab(state * s, int speed);
int release(state * s, int speed);
void set_wheels_speed(state *s, int speed);
void set_wheels_time(state *s, int time);
void set_wheels_pos(state *s, int pos);
void command_wheels(state *s, int cmd);
int wheels_run_time(state *s, int speed, int time);
int wheels_run_pos(state *s, int speed, int pos);
int wheels_run_distance(state *s,int speed, int distance);
int go_to_pos(state * s, position desiredposition);
int go_to_pos_incomplete(state * s, position desiredposition);
int go_straight(state *s,int speed, int distance);
int turn(state *s,int speed,int angle);
int turn_absolute(state *s, int speed, int angle);
int turn_imprecise(state *s,int speed,int angle);
int is_running_in_correct_angle(state *s);
int go_straight_compass(state *s,int speed, int distance);
int turn_compass(state *s,int speed,int angle);
int is_running_in_correct_angle_compass(state *s);
int sweep(state *s, int speed, int angle);


#endif /* INCLUDES_MOTORS_H_ */
