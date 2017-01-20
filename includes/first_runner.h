#ifndef __FIRST_RUNNER_H__
#define __FIRST_RUNNER_H__

#include "main.h"
#include "utils.h"

int beginner_small_stadium_1(state *s, mainpos *p);
int beginner_small_stadium_2(state *s, mainpos *p);
int finisher_small_stadium(state *s, mainpos *p);

int beginner_large_stadium(state *s, mainpos *p);
int finisher_large_stadium(state *s, mainpos *p);

int game_wrapper(state *s, mainpos *p);

#endif
