#ifndef __UTILS_H__
#define __UTILS_H__

#include "main.h"

int init_bluetooth();

int init_inet(state *s);
void close_inet(state *s);

int send_message(state *s, int messageType, ...);
char read_from_server(state *s, char *buffer);

int clean_angle(int angle);

void update_pos(state* s, int * pos, int angle);
void init_pos(state *s);
int clean_angle(int angle);
int compute_distance(int * desiredposition);
int compute_angle(int * desiredposition);
int* compute_relative_position(int *actualposition,int *desiredposition);

#endif

