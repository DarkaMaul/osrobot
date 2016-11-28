#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "main.h"
#include "logger.h"
#include "ev3c.h"
#include "utils.h"
#include "motors.h"
#include "init.h"
#include "robot.h"
#include "sensors.h"
//#include "config.h"

state st;
state* s = &st;

int main(int argc, char *argv[])
{
    //Register signal handler
    signal(SIGINT, signal_handler);

    //Init the LeE
    init_robot(s);

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


    //tests dev in progress
    //grab(s,MAX_GRABBING_SPEED);
    //sleep(5);
    //release(s, MAX_GRABBING_SPEED);

	go_straight(s, 1000, 200);
	//Initialize the "trip"

	//ACTION

	//Reset robot

	//Close external ressources
    nice_exit(s, EXIT_SUCCESS);

	return 0;
}
