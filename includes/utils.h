#ifndef __UTILS_H__
#define __UTILS_H__


int init_bluetooth();

int init_inet(state *s);
void close_inet(state *s);

int send_message(state *s, int messageType, ...);
char read_from_server(state *s, char *buffer);

int load_game_params(state *s, char *buffer);

#endif
