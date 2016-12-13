#include <stdio.h>
#include "ev3c.h"
#include "sensors.h"
#include "main.h"
#include "config.h"
#include "motors.h"
#include "logger.h"
/**

 * Catch the ball if it's in front of the robot
 * @param  s State structure
 * @return   BALL_FOUND || BALL_NOT_FOUND
 */
int catch_ball(state* s)
{
    log_this(s, "[%s] Trying to catch the ball.\n", __FILE__);
    /*
    	Already in release position
    //First release the hook
    //release(s, MAX_GRABBING_SPEED);
	*/
    //Then check if the ball is  really there
    int distanceBall = distance_from_obstacle(s);

    //The ball was not found in a confident interval
    if (distanceBall == -1)
    {
        log_this(s, "[%s] No ball found in front of the robot... :(  \n", __FILE__);
        return -1;
    }

    //Run to the ball
    go_straight(s, MAX_WHEEL_SPEED, distanceBall - BALL_CATCH_DISTANCE);

    //Close the hook
    grab(s, MAX_GRABBING_SPEED);

    int returnValue = is_ball_present(s);
    if(returnValue == BALL_FOUND)
        log_this(s, "[%s] We're lucky, the ball is inside the hook!\n", __FILE__);
    else
        log_this(s, "[%s] Damn, we missed it at the very last moment\n", __FILE__);

    return returnValue;
}
/**
 * Function to look for the ball while moving (or not)
 * @param s State of LeE
 * @return 0 if ball is found 1 otherwise
 */
int look_for_ball(state *s){
	int distanceToBallorObstacle = distance_from_obstacle(s);
	int turn_sweep= 0;
	while(distanceToBallorObstacle == -1 || distanceToBallorObstacle > 50)
	{
		//Positive for clockwise turn
/*
		if (abs(sweep * SWEEP_ANGLE) < MAX_SWEEP_ANGLE)
			sweep(s, TURNING_SPEED, -SWEEP_ANGLE);
		else
			sweep(s, TURNING_SPEED, SWEEP_ANGLE);
*/
		sleep(1);
		distanceToBallorObstacle = distance_from_obstacle(s);
		turn_sweep++;
	}
	return 0;
}
