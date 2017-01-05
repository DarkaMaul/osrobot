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
#include "first_runner.h"
#include "tester.h"

//#include "config.h"

state st;
state* s = &st;

int main(int argc, char *argv[])
{
    //Register signal handler
    signal(SIGINT, signal_handler);

    //Init the LeE
    init_robot(s);
    //init_inet(s);
    look_for_ball(s);
    //tests dev in progress
    //grab(s,MAX_GRABBING_SPEED);
    //sleep(5);
    //release(s, RELEASING_SPEED);

	//go_straight(s, 1000, 200);
//    position desiredposition={2,2};
//    go_to_pos(s, desiredposition);
//    sleep(3);
//    desiredposition.x=1;
//    desiredposition.y=1;
//    go_to_pos(s, desiredposition);
	//Initialize the "trip"

	//ACTION


	//Reset robot

	//13 December
    if(argc == 2){

        if (atoi(argv[1]) != 7)
            init_robot(s);
        else
            init_logger(s);

    	switch(atoi(argv[1])){
			case 1:
				test_one(s);
				break;
			case 2:
				test_two(s);
				break;
			case 3:
				test_three(s);
				break;
			case 4:
				test_four(s);
				break;
			case 5:
				test_five(s);
				break;
            case 6:
                test_six(s);
                break;
            case 7:
                test_bluetooth(s);
                break;

			default:
				beginner_small_stadium(s);
				break;
		}
	}
	//Close external ressources
    
    nice_exit(s, EXIT_SUCCESS);
	return 0;
}
