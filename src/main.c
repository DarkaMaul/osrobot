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
#include "finisher.h"
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

    mainpos *p;
    if (s->type == SMALL_ARENA)
    {
        if(s->role == ROLE_FIRST)
            beginner_small_stadium(s, p);
        else
            finisher_small_stadium(s, p);
    } else
    {
        //TODO
    }

    return;
}

int main(int argc, char *argv[])
{
    //Register signal handler
    signal(SIGINT, signal_handler);

    //Init the LeE
    init_robot(s,p);
    //init_inet(s);
    //look_for_ball(s);
    int distance_to_ball = look_for_ball(s);
    if (distance_to_ball == SONAR_ERROR_ANGLE)
    {
        go_straight(s, MAX_WHEEL_SPEED, 30);
        distance_to_ball = look_for_ball(s);
    }
    //TODO Scan the whole arena if we don't find the ball

    //Turn to face the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Preparing to catch the ball\n\n", __FILE__);
    turn(s,TURNING_SPEED, distance_to_ball);

    //Catch the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Catching the ball\n\n", __FILE__);
    catch_ball(s);


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
