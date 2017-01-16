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
#include <pthread.h>

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

    pthread_mutex_lock(&(s->mutexPosition));
    s->curPos.x=pos.x;
	s->curPos.y=pos.y;
    pthread_mutex_unlock(&(s->mutexPosition));
}

/**
 * Initialize position and angle to predefined values
 * @param  s        State structure
 */
void init_pos(state *s)
{
    position pos = {0,0};
	update_pos(s, pos);
	s->angle=-90;
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

/**
* Initialize the positions used by LeE
*@param positions The positions variable defined in main
**/
void init_main_positions(state *s, mainpos *p){

    int side = s->side;

    position s_fr_init={.x = S_FR_S_0_X,.y = S_FR_S_0_Y +  WHEELS_TO_END};
    position s_fr_ballareaposition = {.x = S_BA_0_X - 5,.y = S_BA_0_Y - WHEELS_TO_END + 5};
    position s_fr_endingposition = {.x = S_FR_E_1_X,.y = S_FR_E_1_Y};
    position l_fr_init = {.x = side*(L_FR_S_0_X + WHEELS_TO_END), .y = L_FR_S_0_Y + WHEELS_TO_END};
    position l_fr_dodgefirst = {.x = side*(L_O1_2_X + BIG_ARENA_MAX_X)/2, .y = L_O1_2_Y + L_FR_S_0_Y};
    position l_fr_center = {.x = side*(L_BA_0_X + L_FR_S_0_X/2), .y = L_BA_0_Y + L_FR_S_0_Y};
    position l_fr_dodgesecond = {.x = side*L_FR_E_0_X , .y = L_O2_0_Y};
    position l_fr_ending = {.x = side*L_FR_E_1_X, .y = L_FR_E_1_Y};


    position s_sr_init={.x = S_SR_S_0_X + ROBOT_WIDTH,.y = S_SR_S_0_Y +  WHEELS_TO_END};
    position s_sr_ballareaposition = {.x = S_BA_1_X - 5,.y = S_BA_1_Y + WHEELS_TO_END + 5};
    position s_sr_endingposition = {.x = S_SR_E_1_X + ROBOT_WIDTH, .y = S_SR_E_1_Y - WHEELS_TO_END};
    position l_sr_init = {.x = side*(L_FR_S_0_X + WHEELS_TO_END), .y = L_FR_S_0_Y + WHEELS_TO_END};
    position l_sr_dodgefirst = {.x = side*(L_O1_2_X + BIG_ARENA_MAX_X)/2, .y = L_O1_2_Y + L_FR_S_0_Y};
    position l_sr_center = {.x = side*(L_BA_0_X + L_FR_S_0_X/2), .y = L_BA_0_Y + L_FR_S_0_Y};
    position l_sr_dodgesecond = {.x = side*L_FR_E_0_X , .y = L_O2_0_Y};
    position l_sr_ending = {.x = side*L_FR_E_1_X, .y = L_FR_E_1_Y};

    p->s_fr_init=s_fr_init;
	p->s_fr_ballarea=s_fr_ballareaposition;
	p->s_fr_ending=s_fr_endingposition;
	p->l_fr_init = l_fr_init;
	p->l_fr_dodgefirst = l_fr_dodgefirst;
	p->l_fr_ballarea = l_fr_center;
	p->l_fr_dodgesecond = l_fr_dodgesecond;
	p->l_fr_ending = l_fr_ending;


    p->s_sr_init=s_sr_init;
    p->s_sr_ballarea=s_sr_ballareaposition;
    p->s_sr_ending=s_sr_endingposition;
    p->l_sr_init = l_sr_init;
    p->l_sr_dodgefirst = l_sr_dodgefirst;
    p->l_sr_ballarea = l_sr_center;
    p->l_sr_dodgesecond = l_sr_dodgesecond;
    p->l_sr_ending = l_sr_ending;
}

/**
 * Compute the point near the ball where the robot will stop
 * @param  s State structure
 * @return   position
 */
position compute_arrival_point(state *s)
{
    position arrivalPoint;
    int norm = compute_distance(compute_relative_position(s->curPos, s->ballPosition));

    int CONSTANT = 10;
    arrivalPoint.x =  s->ballPosition.x - CONSTANT * (s->ballPosition.x - s->curPos.x) / norm;
    arrivalPoint.y =  s->ballPosition.y - CONSTANT * (s->ballPosition.x - s->curPos.y) / norm;

    return arrivalPoint;
}
