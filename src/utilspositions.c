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
int clean_angle(int angle) {
	while (angle < 0) {
		angle = angle + 360;
	}
	return angle % 360;
}

int compute_distance(position desiredposition){
	return (int) floor(sqrt(pow(desiredposition.x,2)+pow(desiredposition.y,2)));
}

int compute_angle(position desiredposition){
	return (int) floor(atan2(desiredposition.x,desiredposition.y));
}

position compute_relative_position(position actualposition,position desiredposition){
	position result;
	result.x=abs(actualposition.x-desiredposition.x);
	result.y=abs(actualposition.y-desiredposition.y);
	return result;
}

int shortest_angle_from_dest(state *s,int desiredangle){
	if(clean_angle(desiredangle+s->angle)>=clean_angle(desiredangle-s->angle)) return desiredangle;
	return -s->angle+desiredangle;
}
