#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "main.h"

int init_sensors(state *s);
int is_ball_present(state *s);
int distance_from_obstacle(state *s);
int gyro_angle(state *s);
int compass_angle(state *s);

#endif
