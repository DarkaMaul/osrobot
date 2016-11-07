#include "ev3c.h"

#include "config.h"


void update_pos(state* s, int * pos, int angle) {
	s->pos=pos;
	s->angle = clean_angle(angle);
	log_printf(s, "\n[UPDATE_POSITION] x=%d y=%d angle=%d\n", s->pos[0], s->pos[1], s->angle);
}

int clean_angle(int angle) {
	while (angle < 0) {
		angle = angle + 360;
	}
	return angle % 360;
}
