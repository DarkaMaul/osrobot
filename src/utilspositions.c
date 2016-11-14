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

void update_pos(state* s, int * pos, int angle) {
	s->pos=pos;
	s->angle = clean_angle(angle);
	log_printf(s, "\n[UPDATE_POSITION] x=%d y=%d angle=%d\n", s->pos[0], s->pos[1], s->angle);
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

