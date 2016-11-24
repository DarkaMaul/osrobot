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
        log_this(s, " Motor %d on port %c opened, assigned and reseted\n",  motor->motor_nr, motor->port);
        motor = motor->next;
    }
    ev3_stop_command_motor_by_name(s->grabmotor, "hold");

    init_pos(s);
    log_this(s, "[%s] Position initialized to x:%d, y:%d\n",__FILE__,s->curPos.x,s->curPos.y);
}

//Grabbing functions

/**
 * Function which can be used to grab the ball at a specified speed
 */
int grab(state *s, int speed)
{
    if (ev3_get_position(s->grabmotor) == GRAB_POSITION){
        log_this(s, "[%s] Grabbing failed already in grabbing position\n", __FILE__);
        return -1;
    }
    ev3_set_speed_sp(s->grabmotor, speed);
    ev3_set_position_sp(s->grabmotor, GRAB_POSITION);
    ev3_command_motor_by_name(s->grabmotor, "run-to-abs-pos");

    while (ev3_motor_state(s->grabmotor) & MOTOR_RUNNING);
    //log_this(s, "Motor %d on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
    return 0;
}


/**
 * Function which can be used to grab the ball at a specified speed
 */
int release(state *s, int speed)
{
    if (ev3_get_position(s->grabmotor) == INIT_GRAB_POSITION){
        log_this(s, "[%s] Releasing failed already in release position\n", __FILE__);
        return -1;
    }
    ev3_set_speed_sp(s->grabmotor, speed);
    ev3_set_position_sp(s->grabmotor, INIT_GRAB_POSITION);
    ev3_command_motor_by_name(s->grabmotor, "run-to-abs-pos");

    while (ev3_motor_state(s->grabmotor) & MOTOR_RUNNING);
    //log_this(s, "Motor %d on port %c opened, assigned and reseted\n", motor->motor_nr, motor->port);
    return 0;
}


//Wheels control functions

/*
 * Functions to set up both wheels at the same time
 */
void set_wheels_speed(state *s, int speed){
    ev3_set_speed_sp(s->leftmotor, speed);
    ev3_set_speed_sp(s->rightmotor, speed);
    log_this(s, "[%s] Wheels' speed set to %d \n", __FILE__, speed);
}

void set_wheels_time(state *s, int time){
    ev3_set_time_sp(s->leftmotor, time);
    ev3_set_time_sp(s->rightmotor, time);
    log_this(s, "[%s] Wheels' time set to %d \n", __FILE__, time);
}

void set_wheels_pos(state *s, int pos){
    ev3_set_position_sp(s->leftmotor, pos);
    ev3_set_position_sp(s->rightmotor, pos);
    log_this(s, "[%s] Wheels' position set to %d \n", __FILE__, pos);
}

/*
 * Function to command both wheels at the same time
 */
void command_wheels(state *s, int cmd){
    ev3_command_motor(s->leftmotor, cmd);
    ev3_command_motor(s->rightmotor, cmd);
}

/*
 * Function to go for a given time at a given speed
 */
int wheels_run_time(state *s, int speed, int time){
    log_this(s, "[%s] Wheels running for %d s ...\n", __FILE__, time);
    set_wheels_speed(s, speed);
    set_wheels_time(s, time);
    command_wheels(s, RUN_TIMED);
    while (ev3_motor_state(s->leftmotor) & MOTOR_RUNNING);
    return 0;
}


/*
 * Function to turn both wheels from a given angle
 */
int wheels_run_pos(state *s, int speed, int pos){
    log_this(s, "[%s] Wheels running to relative position %d...\n", __FILE__, pos);
    set_wheels_speed(s, speed);
    set_wheels_pos(s, pos);
    command_wheels(s, RUN_TO_REL_POS);
    while (ev3_motor_state(s->leftmotor) & MOTOR_RUNNING);
    //TODO update_pos(s, pos);
    return 0;
}

/*
 * Function to go a given distance at a given speed
 */
int wheels_run_distance(state *s, int speed, int distance){
    log_this(s, "[%s] Going Straight for %d cm...\n", __FILE__, distance);
    // deduce the angle from the given distance :
    int position = (distance*360)/(M_PI*WHEEL_DIAMETER);// wheels have diameter 5.6 cm
    return wheels_run_pos(s, speed, position);
}

/**
 * Go straight for a distance in cm
 * @param  s        State structure
 * @param  speed    Speed for  the LeE (usually MAX_WHEEL_SPEED)
 * @param  distance Distance in cm
 * @return          0 if everything is allright
 */
int go_straight(state *s, int speed, int distance)
{
    return wheels_run_distance(s, speed, distance);
}

/*
 * TODO Function that will be used to go to a specified position
 * First version go straight to the position ignoring obstacles
 */
int go_to_pos(state *s,position desiredposition){
    //while((s->curPos.x!=desiredposition.x)&&(s->curPos.y!=desiredposition.y)){
	s->wantedPos=desiredposition;
	position relativeposition=compute_relative_position(s->curPos,desiredposition);
    int distancetodest=compute_distance(relativeposition);
    int angletodest=compute_angle(relativeposition);
    angletodest=angletodest-s->angle;
    if((s->curPos.x>desiredposition.x)&&(s->curPos.y>desiredposition.y)) angletodest+=180;
    if((s->curPos.x<desiredposition.x)&&(s->curPos.y>desiredposition.y)) angletodest=180-angletodest;
    if((s->curPos.x>desiredposition.x)&&(s->curPos.y<desiredposition.y)) angletodest=365-angletodest;
    turn(s, shortest_angle_from_dest(s, angletodest));
    update_angle(s,angletodest);
    go_straight(s, MAX_WHEEL_SPEED, distancetodest);
    //}
    update_pos(s, desiredposition);
    return 0;
}
/**
 * TODO
 * Function that will be used to turn the robot to the desired angle
 */
int sign(int a){
    return a/abs(a);
}



int turn(state *s, int angle){
    log_this(s, "[%s] : Turning from %d degrees...\n", __FILE__, angle );
    int speed = 150;
    int angle_sign = sign(angle);
    speed = speed * angle_sign;
    ev3_update_sensor_val(s->gyro);
    int current_angle = s->gyro->val_data[0].s32 ;
    int goal = current_angle + angle;
    ev3_set_speed_sp(s->leftmotor, speed);
    ev3_set_speed_sp(s->rightmotor, -speed);
    command_wheels(s, RUN_FOREVER);
    while(angle_sign * current_angle < angle_sign * goal){
        ev3_update_sensor_val(s->gyro);
        current_angle = s->gyro->val_data[0].s32 ;
    }
    command_wheels(s, STOP);
    return 0;
}




/**
 * Function to correct the position while moving
 */
int is_running_in_correct_angle(state *s){
	int actualangle=s->gyro->val_data[0].s32;
	//+- ERROR_M degrees is ok
    int angle_diff = s->angle - actualangle;
	if(! (abs(angle_diff) > ERROR_MARGIN)){
		return angle_diff;
	}
	return 0;
}
