#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

typedef struct _state
{
	//External ressources needed
	//Log file
	FILE * logfile_fd;

    //Socket
    int sock;

	/* Position of the robot */
	int pos[2];
	int angle;
    
} state;

#endif
