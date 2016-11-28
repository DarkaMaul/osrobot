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
	s->curPos.x=pos.y;
	log_this(s, "\n[UPDATE_POSITION] x=%d y=%d ", s->curPos.x, s->curPos.x);
}
void init_pos(state *s){
	s->curPos.x=0;
	s->curPos.x=0;
	s->angle=0;
}
void update_angle(state* s,int angle){
	s->angle = clean_angle(angle); //ADD DECLARATIONS TO UTILS.H
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
int compute_angle(position desiredposition){
	return convert_to_degrees((int) floor(atan2(desiredposition.x,desiredposition.y)));
}

position compute_position_from_distance_and_angle(state *s, int distancetopos){
	position relpos;
	relpos.x=s->curPos.x+distancetopos*cos(convert_to_radians((M_PI)/2-s->angle));
	relpos.y=s->curPos.x+distancetopos*sin(convert_to_radians((M_PI)/2-s->angle));
	return relpos;
}

position compute_relative_position(position actualposition,position desiredposition){
	position result;
	result.x=actualposition.x-desiredposition.x;
	result.y=actualposition.y-desiredposition.y;
	return result;
}

int shortest_angle_from_dest(state *s,int desiredangle){
	if(clean_angle(desiredangle+s->angle)>=clean_angle(desiredangle-s->angle)) return desiredangle;
	return -s->angle+desiredangle;
}

int convert_to_radians(int deg){
	return deg*M_PI/180;
}

int convert_to_degrees(int deg){
	return deg*180/M_PI;
}
