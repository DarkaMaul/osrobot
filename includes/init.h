#ifndef __INIT_H__
#define __INIT_H__

#include "main.h"

int init_robot(state *s, mainpos *p);
void nice_exit(state *s, int exitState);
void signal_handler(int signalNumber);
void init_main_positions(mainpos *p);

#endif
