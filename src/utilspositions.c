/*
 * utilspositions.c
 *
 *  Created on: 14 nov. 2016
 *      Author: axel
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <math.h>

#include "main.h"
#include "config.h"
#include "logger.h"
#include "utils.h"

/**
 * Update the current position of the robot
 * @param  s        State structure
 * @param  position    New position of LeE
 */
void update_pos(state* s, position pos) {
    log_this(s, "\n[UPDATE_POSITION] x=%d y=%d from (%d,%d)\n", pos.x, pos.y, s->curPos.x, s->curPos.y);
    s->curPos.x=pos.x;
	s->curPos.y=pos.y;
}

// Since the angle is update with gyro_angle, which is clockwise, s->angle should be initialized to -90 instead of 90
/**
 * Initialize position and angle to predefined values
 * @param  s        State structure
 */
void init_pos(state *s)
{
	s->curPos.x=0;
	s->curPos.x=0;
	s->angle=90;
}

/**
 * Update angle in state
 * @param  s        State structure
 * @param  angle    New angle for LeE
 */
void update_angle(state* s,int angle){
	s->angle = angle;
	log_this(s, "\n[UPDATE_ANGLE] angle=%d\n",  s->angle);
}

/**
 * Put angle between -180 and 180
 * @param  angle angle to 'clean'
 */
int clean_angle(int angle)
{
    while (angle < -180)
        angle += 360;
    while (angle > 180)
        angle -= 360;

    return angle;
}

/**
 * Calculate distance with coordinates
 * @param  position desiredposition
 */
int compute_distance(position desiredposition){
	return (int) floor(sqrt(pow(desiredposition.x,2)+pow(desiredposition.y,2)));
}

/**
 * Calculate angle with coordinates
 * @param  position desiredrelposition
 */
int compute_angle(position desiredrelposition){
	return (int) floor(convert_to_degrees(atan2((double)desiredrelposition.y,(double)desiredrelposition.x)));
}

/**
* Compute position based on distance and angle
* @param s State of LeE
* @param distancetopos Distance to the position
* @return position calculated
*/
position compute_position_from_distance_and_angle(state *s, int distancetopos){
	position relpos;
	//There is a minus s.angle because values of the sensors are clockwise
	relpos.x=s->curPos.x+distancetopos*cos(convert_to_radians(-s->angle));
	relpos.y=s->curPos.y+distancetopos*sin(convert_to_radians(-s->angle));
	return relpos;
}

/**
* Compute relative position based on actual and desired position
* @param actualposition current position
* @param desiredposition Desired next position
* @return relative position
*/
position compute_relative_position(position actualposition,position desiredposition){
	position result;
	result.x=desiredposition.x-actualposition.x;
	result.y=desiredposition.y-actualposition.y;
	return result;
}

/**
* Convert value in degree to radians
* @param deg degree value to convert
* @return radian value
*/
double convert_to_radians(double deg){
	return deg*M_PI/180.0;
}

/**
* Convert value in radians to degree
* @param rad radian value to convert
* @return degree value
*/
double convert_to_degrees(double rad){
	return rad*180.0/M_PI;
}

/**
* Gives the sign of an int (may already be implemented in math.h, TOCHECK)
* @param a The integer whose sign we want
* @return the sign of a
*/
int sign(int a){
    return a/abs(a);
}
