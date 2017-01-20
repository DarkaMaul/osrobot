#ifndef __UTILS_H__
#define __UTILS_H__

#include "main.h"

typedef struct _mainpositions
{
    position s_fr_init;
    position s_fr_ballarea;
    position s_fr_ending;

    position l_fr_init;
    position l_fr_dodgefirst;
    position l_fr_ballarea;
    position l_fr_dodgesecond;
    position l_fr_ending;

    position s_sr_init;
    position s_sr_ballarea;
    position s_sr_ending;

    position l_sr_init;
    position l_sr_dodgefirst;
    position l_sr_ballarea;
    position l_sr_dodgesecond;
    position l_sr_ending;
} mainpos;

int init_bluetooth();

int init_inet(state *s);
void close_inet(state *s);

int send_message(state *s, int messageType, unsigned char destination, ...);
int read_from_server(state *s, char *buffer);
int read_message_from_server(state *s, char *buffer);
int load_game_params(state *s, char *buffer);

int send_position(state *s, position sendedPosition);

int is_in_arena(state *s, position testedPosition);

void update_pos(state* s, position pos);
void update_angle(state* s, int angle);
void init_pos(state *s);
int clean_angle(int angle);
int compute_distance(position desiredposition);
int compute_angle(position desiredposition);
position compute_relative_position(position actualposition,position desiredposition);
int shortest_angle_from_dest(state *s,int desiredangle);
position compute_position_from_distance_and_angle(state *s, int distancetopos);
double convert_to_radians(double deg);
double convert_to_degrees(double deg);
int sign(int a);
int save_ball_position(state *s, char *buff);
void init_main_positions(state *s, mainpos *p);

position compute_arrival_point(state *s);
void send_ball_position(state *s, int action);

#endif
