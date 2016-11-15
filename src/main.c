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
    */
    //
    // char buffer[MSG_MAX_LEN];
    // char messageType = read_from_server(s, buffer);

    //Waiting for the game to start. Ignore all messages not directed to our team.
    /*
    while (messageType != MSG_START)
    {
        if (messageType == NOT_FOR_ME)
            messageType = read_from_server(s, buffer);
        else
            nice_exit(s, EXIT_FAILURE);
    }
    */

    // //Try to parse start message to load the game parameters
    // int returnCode;
    // if (messageType == MSG_START)
    // {
    //     returnCode = load_game_params(s, buffer);
    //     if (returnCode != 0)
    //         nice_exit(s, EXIT_FAILURE);
    // }

    /*
    //tests dev in progress
    init_motors(s);
    grab(s,MAX_GRABBING_SPEED);
    sleep(2);
    release(s, MAX_GRABBING_SPEED);
    */
	//Initialize the "trip"

	//ACTION

	//Reset robot

	//Close external ressources
    nice_exit(s, EXIT_SUCCESS);

	return 0;
}
