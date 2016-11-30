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


void update_pos(state* s, position pos) {
	s->curPos.x=pos.x;
	s->curPos.y=pos.y;
	log_this(s, "\n[UPDATE_POSITION] x=%d y=%d ", s->curPos.x, s->curPos.y);
}
void init_pos(state *s){
	s->curPos.x=0;
	s->curPos.x=0;
	s->angle=0;
}
void update_angle(state* s,int angle){
	s->angle = angle;
	log_this(s, "\n[UPDATE_ANGLE] angle=%d\n",  s->angle);
}

int clean_angle_2(int angle)
{
    while (angle < -180)
        angle += 360;
    while (angle > 180)
        angle -= 360;

    return angle;
}
//TODO deprecated delete it if you don't use it
int clean_angle(int angle) {
	while (angle < 0) {
		angle = angle + 360;
	}
	return angle % 360;
}

int compute_distance(position desiredposition){
	return (int) floor(sqrt(pow(desiredposition.x,2)+pow(desiredposition.y,2)));
}
//TODO Mettre en degres Theta->90-Theta pour calculer distance
int compute_angle(position desiredrelposition){
	return (int) floor(convert_to_degrees(atan2((double)desiredrelposition.y,(double)desiredrelposition.x)));
}

position compute_position_from_distance_and_angle(state *s, int distancetopos){
	position relpos;
	relpos.x=s->curPos.x+distancetopos*cos(convert_to_radians(s->angle));
	relpos.y=s->curPos.y+distancetopos*sin(convert_to_radians(s->angle));
	return relpos;
}

position compute_relative_position(position actualposition,position desiredposition){
	position result;
	result.x=desiredposition.x-actualposition.x;
	result.y=desiredposition.y-actualposition.y;
	return result;
}
//TODO deprecated
int shortest_angle_from_dest(state *s,int desiredangle){
	if(clean_angle(desiredangle+s->angle)>=clean_angle(desiredangle-s->angle)) return desiredangle;
	return -s->angle+desiredangle;
}

double convert_to_radians(double deg){
	return deg*M_PI/180.0;
}

double convert_to_degrees(double rad){
	return rad*180.0/M_PI;
}
