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

#include "main.h"
#include "config.h"
#include "logger.h"
#include "utils.h"
#include "math.h"

void update_pos(state* s, int * pos, int angle) {
	//s->pos=pos; CAREFUL s->pos is int[2]
	s->angle = clean_angle(angle); //ADD DECLARATIONS TO UTILS.H
	log_this(s, "\n[UPDATE_POSITION] x=%d y=%d angle=%d\n", s->pos[0], s->pos[1], s->angle);
}
void init_pos(state *s){
	s->pos[0]=0;
	s->pos[1]=1;
	s->angle=0;
}

int clean_angle(int angle) {
	while (angle < 0) {
		angle = angle + 360;
	}
	return angle % 360;
}

int compute_distance(int * desiredposition){
	return (int) floor(sqrt(pow(desiredposition[0],2)+pow(desiredposition[1],2)));
}

int compute_angle(int * desiredposition){
	return (int) floor(atan2(desiredposition[0],desiredposition[1]));
}

