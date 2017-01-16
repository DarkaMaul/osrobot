#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "main.h"
#include "logger.h"
#include "ev3c.h"
#include "utils.h"
#include "motors.h"
#include "init.h"
#include "robot.h"
#include "sensors.h"
#include "first_runner.h"
#include "tester.h"
#include "config.h"

state st;
state* s = &st;

mainpos mpos;
mainpos* p = &mpos;

void game()
{
    //Game function o_OOOOO_o

    //Let's wait for the starting message
    pthread_mutex_lock(&(s->mutexSockUsage));

    char buf[100];
    int returnValue;
    while(1)
    {
        returnValue  = read_message_from_server(s, buf);
        if (returnValue == -1) //We don't release the lock because we're gonna die anyway
            continue;

        if (buf[HEADER_TYPE] == MSG_START)
            break;
    }

    pthread_mutex_unlock(&(s->mutexSockUsage));

    load_game_params(s, buf);
    game_wrapper(s, p);

    return;
}

int main(int argc, char *argv[])
{
    //Register signal handler
    signal(SIGINT, signal_handler);

	//Reset robot

	//13 December
    if(argc == 2){
    	switch(atoi(argv[1])){
    		//Test bluetooth
            case 0:
            	if (atoi(argv[1]) != 7)
            		init_robot(s,p);
            	else
            		init_logger(s);
                test_bluetooth(s);
                break;
            //Run beginner
            case 1:
            	init_robot(s,p);
            	beginner_small_stadium(s,p);
            	look_for_ball(s);
            	break;
            //Test finisher
            case 2:
            	init_robot(s,p);
            	finisher_small_stadium(s, p);
            	break;
            //Quicks tests
            case 3:
        		init_robot(s,p);
				look_for_ball(s);
				break;
		}
	}
	//Close external ressources

    nice_exit(s, EXIT_SUCCESS);
	return 0;
}
