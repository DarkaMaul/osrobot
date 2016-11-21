#include "motors.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>


#include "logger.h"
#include "config.h"
#include "utils.h"


void init_motors(state *s){
    s->motors = ev3_load_motors();
    s->leftmotor=ev3_search_motor_by_port(s->motors, *PORT_LEFT_MOTOR);
    s->rightmotor=ev3_search_motor_by_port(s->motors, *PORT_RIGHT_MOTOR);
    s->grabmotor=ev3_search_motor_by_port(s->motors, *PORT_GRAB_MOTOR);
    s->sweepmotor=ev3_search_motor_by_port(s->motors, *PORT_SWEEP_MOTOR);
    ev3_motor_ptr motor = s->motors;
    //Just in case reset all motors
    while (motor)
    {
        ev3_open_motor(motor);
        ev3_reset_motor(motor);
        ev3_set_position(motor, 0);
        log_this(s, "%s Motor %i on port %c opened, assigned and reseted\n", __FILE__,  motor->motor_nr, motor->port);
        motor = motor->next;
    }
    ev3_stop_command_motor_by_name(s->grabmotor, "hold");

    init_pos(s);
    log_this(s, "Position initialized to x:%i, y:%i\n", __FILE__,s->pos[0],s->pos[1]);
}

//Grabbing functions

/**
 * Function which can be used to grab the ball at a specified speed
 */
int grab(state *s, int speed)
{
    if (ev3_get_position(s->grabmotor) == GRAB_POSITION){
        log_this(s, "%s Grabbing failed already in grabbing position\n", __FILE__);
        return -1;
    }
    ev3_set_speed_sp(s->grabmotor, speed);
    ev3_set_position_sp(s->grabmotor, GRAB_POSITION);
    ev3_command_motor_by_name(s->grabmotor, "run-to-abs-pos");

    while (ev3_motor_state(s->grabmotor) & MOTOR_RUNNING);
    //log_this(s, "Motor %i on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
    return 0;
}


/**
 * Function which can be used to grab the ball at a specified speed
 */
int release(state *s, int speed)
{
    if (ev3_get_position(s->grabmotor) == INIT_GRAB_POSITION){
        log_this(s, "%s Releasing failed already in grabbing position\n", __FILE__);
        return -1;
    }
    ev3_set_speed_sp(s->grabmotor, speed);
    ev3_set_position_sp(s->grabmotor, INIT_GRAB_POSITION);
    ev3_command_motor_by_name(s->grabmotor, "run-to-abs-pos");

    while (ev3_motor_state(s->grabmotor) & MOTOR_RUNNING);
    //log_this(s, "Motor %i on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
    return 0;
}


//Wheels control functions

/*
 * Functions to set up both wheels at the same time
 */
void set_wheels_speed(state *s, int speed){
    ev3_set_speed_sp(s->leftmotor, speed);
    ev3_set_speed_sp(s->rightmotor, speed);
    log_this(s, "%s Wheels' speed set to %d \n", __FILE__, speed);
}

void set_wheels_time(state *s, int time){
    ev3_set_time_sp(s->leftmotor, time);
    ev3_set_time_sp(s->rightmotor, time);
    log_this(s, "%s Wheels' time set to %d \n", __FILE__, time);
}

void set_wheels_pos(state *s, int pos){
    ev3_set_position_sp(s->leftmotor, pos);
    ev3_set_position_sp(s->rightmotor, pos);
    log_this(s, "%s Wheels' position set to %d \n", __FILE__, pos);
}

/*
 * Function to command both wheels at the same time
 */
void command_wheels(state *s, int cmd){
    ev3_command_motor(s->leftmotor, cmd);
    ev3_command_motor(s->rightmotor, cmd);
    while (ev3_motor_state(s->leftmotor) & MOTOR_RUNNING);
}

/*
 * Function to go for a given time at a given speed
 */
int wheels_run_time(state *s, int speed, int time){
    log_this(s, "%s Wheels running for %d s ...", __FILE__, time);
    set_wheels_speed(s, speed);
    set_wheels_time(s, time);
    command_wheels(s, RUN_TIMED);
    log_this(s, "Done\n");
    return 0;
}


/*
 * Function to go a given distance at a given speed
 */
int wheels_run_pos(state *s, int speed, int pos){
    log_this(s, "%s Wheels running to relative position %d...", __FILE__, pos);
    set_wheels_speed(s, speed);
    set_wheels_pos(s, pos);
    command_wheels(s, RUN_TO_REL_POS);
    log_this(s, "Done\n");
    return 0;
}

/*
 * Function to go straight
 */
int go_straight(state *s, int speed, int distance){
    log_this(s, "%s Going Straight for %d cm...", __FILE__, distance);
    // deduce the angle from the given distance :
    int position = (distance*360)/(M_PI*WHEEL_DIAMETER);// wheels have diameter 5.6 cm
    return wheels_run_pos(s, speed, position);
}


/*
 * TODO Function that will be used to go to a specified position
 * First version go straight to the position ignoring obstacles
 */
int go_to_pos(state *s,int *desiredposition){
    //while((s->pos[0]!=desiredposition[0])&&(s->pos[1]!=desiredposition[1])){
	int *relativeposition=compute_relative_position(s->pos,desiredposition);
    int distancetodest=compute_distance(relativeposition);
    int angletodest=compute_angle(relativeposition);
    free(relativeposition);
    angletodest=angletodest-s->angle;
    if((s->pos[0]>desiredposition[0])&&(s->pos[1]>desiredposition[1])) angletodest+=180;
    if((s->pos[0]<desiredposition[0])&&(s->pos[1]>desiredposition[1])) angletodest=180-angletodest;
    if((s->pos[0]>desiredposition[0])&&(s->pos[1]<desiredposition[1])) angletodest=365-angletodest;
    turn_to_desired_angle(s, angletodest);
    go_straight(s, MAX_WHEEL_SPEED, distancetodest);
    //}
    update_pos(s, desiredposition, angletodest);
    return 0;
}
/**
 * TODO
 * Function that will be used to turn the robot to the desired angle
 */
int turn_to_desired_angle(state *s,int angle){
	return 0;
}

/**
 * Function to correct the position while moving
 */
int is_running_in_correct_angle(state *s){
	int actualangle=s->gyro->val_data[0].s32;
	//+- ERROR_M degrees is ok
	if(!((s->angle-ERROR_MARGIN<actualangle)||(actualangle<s->angle+ERROR_MARGIN))){
		return 1;
	}
	return 0;
}
