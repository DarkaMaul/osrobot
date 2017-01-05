#include <stdio.h>
#include <math.h>
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
    go_straight(s, MAX_WHEEL_SPEED/10, distanceBall - BALL_CATCH_DISTANCE);

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
 * Function to look for the ball when close to the ball area
 * @param s State of LeE
 * @return angle if ball is found SONAR_ERROR_ANGLE otherwise
 */
int look_for_ball(state *s){
	int distanceToBallorObstacle = distance_from_obstacle(s);
	int turn_sweep= MAX_SWEEP_ANGLE;
    log_this(s, "[%s] Look for ball started\n", __FILE__);
    log_this(s, "[%s] Distance to ball or obstacle %d\n", __FILE__, distanceToBallorObstacle);
	sweep_absolute(s, 100, MAX_SWEEP_ANGLE);
	int sweep_angle=-SWEEP_ANGLE;
	while((distanceToBallorObstacle == -1 || distanceToBallorObstacle > 50) && abs(turn_sweep) <= MAX_SWEEP_ANGLE)
	{
		turn_sweep+=sweep_angle;
		//Positive for clockwise turn
		sweep_absolute(s, 100, turn_sweep);
		usleep(200000);
		distanceToBallorObstacle = distance_from_obstacle(s);
	    log_this(s, "[%s] Distance to ball %d\n", __FILE__, distanceToBallorObstacle);
	}
	if(abs(turn_sweep) > MAX_SWEEP_ANGLE) return SONAR_ERROR_ANGLE;

	//angle one detected is the first angle where the ball appears
	int angle_one_detected=turn_sweep;
    log_this(s, "[%s] First angle where ball is detected %d\n", __FILE__, angle_one_detected);

    int extra_max_sweep_angle=MAX_SWEEP_ANGLE+10;
	while(distanceToBallorObstacle < 50 && abs(turn_sweep) < extra_max_sweep_angle && distanceToBallorObstacle!=-1)
	{
		turn_sweep+=sweep_angle;
		//Positive for clockwise turn added 20 degrees if ball is in the limit of the sweep angle
		sweep_absolute(s, 100, turn_sweep);
		usleep(200000);
		distanceToBallorObstacle = distance_from_obstacle(s);
		log_this(s, "[%s] Distance to ball or obstacle %d\n", __FILE__, distanceToBallorObstacle);
	}
	int angle_two_lost=turn_sweep;
    log_this(s, "[%s] Second angle where ball is not detected anymore %d\n", __FILE__, angle_two_lost);
    int bissect_angle=(angle_one_detected-angle_two_lost)/2;
    log_this(s, "[%s] Calculated bissectrice to turn to detect ball %d\n", __FILE__, angle_two_lost);
	//replace the motors to the original position
    sweep_absolute(s, 100, MAX_SWEEP_ANGLE);
    return bissect_angle;
}

/**
 * Function to look for an obstacle while moving (or not)
 * @param s State of LeE
 * @return 0 if obstacle is found 1 otherwise
 */
/*
int look_for_obstacle(state *s){
	int distanceToBallorObstacle = distance_from_obstacle(s);
	int turn_sweep= MAX_SWEEP_ANGLE;
    log_this(s, "[%s] Look for obstacle started\n", __FILE__);
    log_this(s, "[%s] Distance to obstacle or obstacle %d\n", __FILE__, distanceToBallorObstacle);
	sweep_absolute(s, 100, MAX_SWEEP_ANGLE);
	int sweep_angle=-SWEEP_ANGLE;
	while(distanceToBallorObstacle == -1 || distanceToBallorObstacle > 50)
	{
		turn_sweep+=sweep_angle;
		//Positive for clockwise turn
		if (abs(turn_sweep) < MAX_SWEEP_ANGLE){
			sweep_absolute(s, 100, turn_sweep);
		}
		else{
			sweep_angle=-sweep_angle;
		}
		usleep(200000);
		distanceToBallorObstacle = distance_from_obstacle(s);
	    log_this(s, "[%s] Distance to ball or obstacle %d\n", __FILE__, distanceToBallorObstacle);
	}
	//angle one detected is the first angle where the obstacle appears
	int angle_one_detected=turn_sweep;
    return 0;
}
*/


