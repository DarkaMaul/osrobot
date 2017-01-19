#ifndef __INIT_H__
#define __INIT_H__

#include "main.h"
#include "utils.h"

int init_robot(state *s);
void nice_exit(state *s, int exitState);
int init_locks(state *s);
void signal_handler(int signalNumber);

#endif
