#ifndef __INIT_H__
#define __INIT_H__

#include "main.h"

int init_robot(state *s);
void nice_exit(state *s, int exitState);
void signal_handler(int signalNumber);

#endif
