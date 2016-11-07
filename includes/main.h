#ifndef MAIN_H
#define MAIN_H

typedef struct _state
{
	/* log file fd */
	FILE * logfile_fd;
	//Position of the robot
	int x;
	int y;
	int angle;
} state;

#endif
