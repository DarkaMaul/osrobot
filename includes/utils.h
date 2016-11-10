#ifndef __UTILS_H__
#define __UTILS_H__


int init_bluetooth();

int init_inet(state *s);
void close_inet(state *s);

int read_from_server(state *s, char *buffer);
int send_message(state *s, int messageType, ...);

#endif
