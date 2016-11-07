#ifndef MAIN_H
#define MAIN_H

typedef struct _state
{
	/* log file fd */
	FILE * logfile_fd;
	/* Position of the robot */
	int pos;
	int angle;

    //Bluetooth
    int sock;
    int lastAck;
} state;

#endif
