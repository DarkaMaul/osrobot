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

//#include "config.h"

state st;
state* s = &st;

void game()
{
    //Game function o_OOOOO_o

    //Let's wait for the starting message

}

mainpos mpos;
mainpos* p = &mpos;

int main(int argc, char *argv[])
{
    //Register signal handler
    signal(SIGINT, signal_handler);


	//Reset robot

	//13 December
    if(argc == 2){

        if (atoi(argv[1]) != 7)
            init_robot(s,p);
        else
            init_logger(s);

    	switch(atoi(argv[1])){
			case 1:
				test_one(s,p);
				break;
			case 2:
				test_two(s,p);
				break;
			case 3:
				test_three(s,p);
				break;
			case 4:
				test_four(s,p);
				break;
			case 5:
				test_five(s,p);
				break;
            case 6:
                test_six(s,p);
                break;
            case 7:
                test_bluetooth(s);
                break;

			default:
				beginner_small_stadium(s,p);
				break;
		}
	}
	//Close external ressources

    nice_exit(s, EXIT_SUCCESS);
	return 0;
}
