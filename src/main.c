#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#include "logger.h"
#include "ev3c.h"
#include "utils.h"
#include "motors.h"
//#include "config.h"

state st;
state* s = &st;

void nice_exit(state *s, int exitState)
{
    log_this(s, "[Main] Nice exit called.\n");

    if (s->sock > 0)
        close_inet(s);

    if (s->logfile_fd > 0)
        close_logger(s);
    ev3_delete_motors(s->motors);
    exit(exitState);
}


int main(int argc, char *argv[])
{
	//Initialize external ressources
	init_logger(s);

    /*s->sock = init_inet(s);
    if (s->sock == -1)
        nice_exit(s, EXIT_FAILURE);

    send_message(s, MSG_NEXT);
    send_message(s, MSG_ACK, 0x01, ACK_OK);*/

	//Initialize the robot
	//Capteurs Moteur "Connexion"

    //Initialize connexion


    //tests dev in progress
	/*init_motors(s);
	grab(s,MAX_GRABBING_SPEED);
	sleep(2);
	release(s, MAX_GRABBING_SPEED);*/
	init_pos(s);
	go_straight(s, 300, 1000);
	//Initialize the "trip"

	//ACTION

	//Reset robot

	//Close external ressources
    nice_exit(s, EXIT_SUCCESS);

	return 0;
}
