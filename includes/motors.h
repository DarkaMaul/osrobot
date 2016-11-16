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
int go_to_position(state * s, int speed);
int go_straight(state *s,int speed, int distance);

#endif /* INCLUDES_MOTORS_H_ */
