#ifndef __UTILS_H__
#define __UTILS_H__

#include "main.h"

int init_bluetooth();

int init_inet(state *s);
void close_inet(state *s);

int send_message(state *s, int messageType, ...);
char read_from_server(state *s, char *buffer);

int load_game_params(state *s, char *buffer);

bool is_in_arena(state *s, position testedPosition);

int clean_angle(int angle);

void update_pos(state* s, position pos);
void update_angle(state* s, int angle);
void init_pos(state *s);
int clean_angle(int angle);
int compute_distance(position desiredposition);
int compute_angle(position desiredposition);
position compute_relative_position(position actualposition,position desiredposition);
int shortest_angle_from_dest(state *s,int desiredangle);
position compute_position_from_distance_and_angle(state *s, int distancetopos);
int convert_to_radians(int deg);
int convert_to_degrees(int deg);


int clean_angle_2(int angle);

#endif
