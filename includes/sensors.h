#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "main.h"

int init_sensors(state *s);
int is_ball_present(state *s);
int distance_from_obstacle(state *s);

#endif
