#include <stdio.h>
#include <math.h>
#include "ev3c.h"
#include "sensors.h"
#include "main.h"
#include "config.h"
#include "motors.h"
#include "logger.h"
#include "utils.h"

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
        log_this(s, "[%s] No ball found in front of the robot... :(  \n", __FILE__);
        return -1;
    }

    //Run to the ball
    go_straight(s, MAX_WHEEL_SPEED/10, distanceBall - BALL_CATCH_DISTANCE);

    //Close the hook
    grab(s, MAX_GRABBING_SPEED);

    int returnValue = is_ball_present(s);
    if(returnValue == BALL_FOUND){
        log_this(s, "[%s] We're lucky, the ball is inside the hook!\n", __FILE__);
        send_message(s, MSG_BALL, s->ally);
    }
    else
        log_this(s, "[%s] Damn, we missed it at the very last moment\n", __FILE__);

    return returnValue;
}

/**
 * Function to look for the ball when close to the ball area
 * @param s State of LeE
 * @return angle if ball is found SONAR_ERROR_ANGLE otherwise
 */
int look_for_ball_in_close_perimeter_mecanical(state *s){

    //TODO REPLACE 90 with MAx sweep angle
    int tobereplaced=80;
    turn_imprecise(s, TURNING_SPEED, -tobereplaced);

    int distanceToBallorObstacle = distance_from_obstacle(s);
    log_this(s, "[%s] Look for ball started\n", __FILE__);
    log_this(s, "[%s] Distance to ball or obstacle %d\n", __FILE__, distanceToBallorObstacle);
    int sweep_angle=SWEEP_ANGLE;
    int turn_sweep=-tobereplaced;
    while((distanceToBallorObstacle == -1 || distanceToBallorObstacle > GAP_MIN_BETWEEN_ROBOT_BALL) && abs(turn_sweep) <= tobereplaced)
    {
        turn_sweep+=sweep_angle;
        //Positive for clockwise turn
        turn_imprecise(s, TURNING_SPEED, sweep_angle);
        usleep(200000);
        distanceToBallorObstacle = distance_from_obstacle(s);
        log_this(s, "[%s] Distance to ball %d\n", __FILE__, distanceToBallorObstacle);
    }
    if(abs(turn_sweep) > tobereplaced){
        turn_imprecise(s, TURNING_SPEED, -tobereplaced);
        //To be aligned to go straight after
        return SONAR_ERROR_ANGLE; //Ball not found
    }

    int angle_one_detected=turn_sweep;

    int extra_max_sweep_angle=tobereplaced+10;
    while(distanceToBallorObstacle <= GAP_MIN_BETWEEN_ROBOT_BALL && abs(turn_sweep) < extra_max_sweep_angle && distanceToBallorObstacle!=-1)
    {
        turn_sweep+=sweep_angle;
        //Positive for clockwise turn added 20 degrees if ball is in the limit of the sweep angle
        turn_imprecise(s, TURNING_SPEED, sweep_angle);
        usleep(200000);
        distanceToBallorObstacle = distance_from_obstacle(s);
        log_this(s, "[%s] Distance to ball or obstacle %d\n", __FILE__, distanceToBallorObstacle);
    }
    int angle_two_lost=turn_sweep;
    log_this(s, "[%s] Second angle where ball is not detected anymore %d\n", __FILE__, angle_two_lost);

    //Calculated bisector based on the position of the robot
    int bissect_angle= (angle_one_detected+angle_two_lost-sweep_angle)/2;
    log_this(s, "[%s] Calculated bisector to turn to detect ball %d\n", __FILE__, bissect_angle);

    //replace the motors to the original position
    //sweep_absolute(s, 100, 0);
    int turn_angle=-angle_two_lost+bissect_angle-ERROR_DISTANCE_MARGIN;
    turn_imprecise(s, TURNING_SPEED, turn_angle);
    return bissect_angle;
}

int look_for_ball_mecanical(state *s){
    log_this(s, "\n\n[%s] Look for ball started\n", __FILE__);
    int angle_to_ball = look_for_ball_in_close_perimeter_mecanical(s);

    //TODO parameters can be adjusted to search more
    int nb_of_steps=2;
    int size_of_steps=20;
    int i;
    for (i=0;i<nb_of_steps;i++){
        if (angle_to_ball == SONAR_ERROR_ANGLE)
        {
            go_straight(s, MAX_WHEEL_SPEED, size_of_steps);
            angle_to_ball = look_for_ball_in_close_perimeter_mecanical(s);
        }else{
            log_this(s, "[%s] Look for ball succeeded there is something close to the robot\n", __FILE__);
        }
    }
    return 0;
}

/**
 * Function to look for the ball when close to the ball area
 * @param s State of LeE
 * @return angle if ball is found SONAR_ERROR_ANGLE otherwise
 */
int look_for_ball_in_close_perimeter(state *s){
    sweep_absolute(s, 100, MAX_SWEEP_ANGLE);
    int distanceToBallorObstacle = distance_from_obstacle(s);
    int turn_sweep= -MAX_SWEEP_ANGLE;
    log_this(s, "[%s] Look for ball started\n", __FILE__);
    log_this(s, "[%s] Distance to ball or obstacle %d\n", __FILE__, distanceToBallorObstacle);
    int sweep_angle=+SWEEP_ANGLE;
    while((distanceToBallorObstacle == -1 || distanceToBallorObstacle > GAP_MIN_BETWEEN_ROBOT_BALL) && abs(turn_sweep) <= MAX_SWEEP_ANGLE)
    {
        turn_sweep+=sweep_angle;
        //Positive for clockwise turn
        sweep_absolute(s, 100, turn_sweep);
        usleep(200000);
        distanceToBallorObstacle = distance_from_obstacle(s);
        log_this(s, "[%s] Distance to ball %d\n", __FILE__, distanceToBallorObstacle);
    }
    if(abs(turn_sweep) > MAX_SWEEP_ANGLE){
        sweep_absolute(s, 100, 0);
        return SONAR_ERROR_ANGLE;
    }
    //angle one detected is the first angle where the ball appears
    int angle_one_detected=turn_sweep;
    log_this(s, "[%s] First angle where ball is detected %d\n", __FILE__, angle_one_detected);
    sweep_absolute(s, 100, 0);
    return angle_one_detected;
    /*
    //Angle is too close to the limit so return the first angle found
    if(abs(angle_one_detected+ERROR_DISTANCE_MARGIN) >= MAX_SWEEP_ANGLE){
        int bissect_angle= -angle_one_detected+15;
        log_this(s, "[%s] Not calculating second angle ball is too close to the limit of the sweeper capability. Ball at angle=%d\n", __FILE__, bissect_angle);
        sweep_absolute(s, 100, 0);
        return bissect_angle;
    }

    int extra_max_sweep_angle=MAX_SWEEP_ANGLE+10;
    while(distanceToBallorObstacle <= GAP_MIN_BETWEEN_ROBOT_BALL && abs(turn_sweep) < extra_max_sweep_angle && distanceToBallorObstacle!=-1)
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

    int bissect_angle= -(angle_one_detected+angle_two_lost-sweep_angle)/2-10;
    log_this(s, "[%s] Calculated bisector to turn to detect ball %d\n", __FILE__, bissect_angle);

    //replace the motors to the original position
    sweep_absolute(s, 100, 0);
    return bissect_angle;
     */
}

/**
 * Function to look for the ball anywhere close to the ball area //TODO Anywhere in the arena
 * @param s State of LeE
 * @return 0 if ball is found and is in front of the robot -1 otherwise
 */
int look_for_ball(state *s){
    log_this(s, "\n\n[%s] Look for ball started\n", __FILE__);
    int angle_to_ball = look_for_ball_in_close_perimeter(s);

    //TODO parameters can be adjusted to search more
    int nb_of_steps=1;
    int size_of_steps=15;
    int i;
    for (i=0;i<nb_of_steps;i++){
        if (angle_to_ball == SONAR_ERROR_ANGLE)
        {
            go_straight(s, MAX_WHEEL_SPEED, size_of_steps);
            angle_to_ball = look_for_ball_in_close_perimeter(s);
        }else{
            //Turn to face the ball
            log_this(s, "[%s] Turning to be in front of the ball of %d degrees\n", __FILE__, angle_to_ball);
            turn(s,TURNING_SPEED, angle_to_ball);
            log_this(s, "[%s] Checking that there is something in front of the robot\n", __FILE__);
            int distanceToBallorObstacle = distance_from_obstacle(s);
            if (distanceToBallorObstacle > GAP_MIN_BETWEEN_ROBOT_BALL){
                log_this(s, "[%s] Look for ball go for another iteration there is nothing in front of the robot\n", __FILE__);
                angle_to_ball = look_for_ball_in_close_perimeter(s);
            }
            else{
                log_this(s, "[%s] Look for ball succeeded there is something close to the robot\n", __FILE__);
                break;
            }
        }
    }

    //TODO Scan the whole arena if we don't find the ball
    /*
    //Turn to face the ball
    log_this(s, "[%s] Turning to be in front of the ball of %d degrees\n", __FILE__, angle_to_ball);
    turn(s,TURNING_SPEED, angle_to_ball);
    log_this(s, "[%s] Checking that there is something in front of the robot\n", __FILE__);
    int distanceToBallorObstacle = distance_from_obstacle(s);
    if (distanceToBallorObstacle > GAP_MIN_BETWEEN_ROBOT_BALL){
        log_this(s, "[%s] Look for ball failed there is nothing in front of the robot\n", __FILE__);
        return -1;
    }
    log_this(s, "[%s] Look for ball succeeded there is something close to the robot\n", __FILE__);
     */
    return 0;
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
	while(distanceToBallorObstacle == -1 || distanceToBallorObstacle > GAP_MIN_BETWEEN_ROBOT_BALL)
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
