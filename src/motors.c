#include "motors.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "logger.h"
#include "config.h"
#include "utils.h"
#include "sensors.h"

/**
 * Init all the motors of the robot
 * @param  s        State structure
 */
void init_motors(state *s){
    s->motors = ev3_load_motors();
    s->leftmotor=ev3_search_motor_by_port(s->motors, *PORT_LEFT_MOTOR);
    s->rightmotor=ev3_search_motor_by_port(s->motors, *PORT_RIGHT_MOTOR);
    s->grabmotor=ev3_search_motor_by_port(s->motors, *PORT_GRAB_MOTOR);
    //s->sweepmotor=ev3_search_motor_by_port(s->motors, *PORT_SWEEP_MOTOR);
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

    //Ramp smoothly to max speed
    ev3_set_ramp_up_sp(s->rightmotor, 500);
    ev3_set_ramp_up_sp(s->leftmotor, 500);
    ev3_set_ramp_down_sp(s->rightmotor, 1000);
    ev3_set_ramp_down_sp(s->leftmotor, 1000);

}
//Grab control functions

/**
 * Function which can be used to grab the ball at a specified speed
 * @param  s        State structure
 * @param  speed    Speed for LeE (usually MAX_WHEEL_SPEED)
 * @return          0 if everything is allright -1 else
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
 * Function which can be used to release the ball at a specified speed
 * @param  s        State structure
 * @param  speed    Speed for LeE (usually MAX_WHEEL_SPEED)
 * @return          0 if everything is allright -1 else
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

/**
* Set both wheels at the same speed
* @param s     State of LeE
* @param speed Speed to set
*/
void set_wheels_speed(state *s, int speed){
    ev3_set_speed_sp(s->leftmotor, speed);
    ev3_set_speed_sp(s->rightmotor, speed);
    log_this(s, "[%s] Wheels' speed set to %d \n", __FILE__, speed);
}

/**
* Set the same running time for both wheels
* @param s    State of LeE
* @param time Time to set in seconds
*/
void set_wheels_time(state *s, int time){
    ev3_set_time_sp(s->leftmotor, time);
    ev3_set_time_sp(s->rightmotor, time);
    log_this(s, "[%s] Wheels' time set to %d \n", __FILE__, time);
}

/**
* Set the same position goal (absolute angle) for both wheels
* @param s   State of LeE
* @param pos Position to set in degrees
*/
void set_wheels_pos(state *s, int pos){
    ev3_set_position_sp(s->leftmotor, pos);
    ev3_set_position_sp(s->rightmotor, pos);
    log_this(s, "[%s] Wheels' position set to %d \n", __FILE__, pos);
}

/**
* Command both wheels at the same time
* @param s   State of LeE
* @param cmd Command to send to the motors (see the config file for a list of available commands)
*/
void command_wheels(state *s, int cmd){
    ev3_command_motor(s->leftmotor, cmd);
    ev3_command_motor(s->rightmotor, cmd);
}

/**
* Go for a given time at a given speed
* @param s     State of LeE
* @param speed Speed of LeE
* @param time  Running time in seconds
* @return 0 if everything is alright
*/
int wheels_run_time(state *s, int speed, int time){
    log_this(s, "[%s] Wheels running for %d s ...\n", __FILE__, time);
    set_wheels_speed(s, speed);
    set_wheels_time(s, time);
    command_wheels(s, RUN_TIMED);
    while (ev3_motor_state(s->leftmotor) & MOTOR_RUNNING);
    return 0;
}


/**
* Go to a given position (abolute angle of the motor) at a given speed
* @param s     State of LeE
* @param speed Speed of LeE
* @param pos   Absolute position of the motor to go to in degrees
* @return 0 if everything is alright
*/
int wheels_run_pos(state *s, int speed, int pos){
    log_this(s, "[%s] Wheels running to relative position %d...\n", __FILE__, pos);
    set_wheels_speed(s, speed);
    set_wheels_pos(s, pos);
    command_wheels(s, RUN_TO_REL_POS);
    while (ev3_motor_state(s->leftmotor) & MOTOR_RUNNING);
    return 0;
}

/**
* Go a given distance at a given speed. Note that LeE won't go straigth : To do so you need to use go_straight.
* @param s        State of LeE
* @param speed    Speed of LeE
* @param distance Distance to run in cm
* @return the result of wheels_run_pos (0 if everything is alright)
*/
int wheels_run_distance(state *s, int speed, int distance){
    // deduce the angle from the given distance :
    int position = (distance*360)/(M_PI*WHEEL_DIAMETER);// wheels have diameter 5.6 cm
    //Approximation that the angle should remains the same
    update_pos(s, compute_position_from_distance_and_angle(s, distance));
    return wheels_run_pos(s, speed, position);
}

/**
 * Go straight for a distance in cm
 * @param  s        State structure
 * @param  speed    Speed for  the LeE (usually MAX_WHEEL_SPEED)
 * @param  distance Distance in cm
 * @return          0 if everything is allright
 */
int go_straight(state *s, int speed, int distance){
    log_this(s, "[%s] : Going straigth for%d cm\n", __FILE__, distance);
    update_angle(s,gyro_angle(s));

    // We divide the wanted distance in steps od STEPLENGTH
	int nb_of_steps = distance / STEPLENGTH;
    int remaining_distance = distance % STEPLENGTH;
	printf("STP: %d \t RD: %d\n", nb_of_steps, remaining_distance);
    int i;
    // After each step we correct the direction of the robot
    for (i=0; i<nb_of_steps; i++){
        wheels_run_distance(s, speed, STEPLENGTH);
        turn(s, TURNING_SPEED, is_running_in_correct_angle(s));
    }
    // We then go for the remaining distance and correct the angle again
    wheels_run_distance(s, speed, remaining_distance);
    turn(s, TURNING_SPEED, is_running_in_correct_angle(s));
    return 0;
}

/*
 * TODO Function that will be used to go to a specified position
 * First version go straight to the position ignoring obstacles
 */
/**
 * Function to go to a specified position
 * @param  s        State structure
 * @param  position    Desired x,y position to go
 * @return          0 if everything is allright
 */
int go_to_pos(state *s, position desiredposition){
	s->wantedPos=desiredposition;
	position relativeposition=compute_relative_position(s->curPos,desiredposition);
	int distancetodest=compute_distance(relativeposition);
	log_this(s, "[%s] Distance relative from destination %d...\n", __FILE__, distancetodest);
	int absoluteangle=compute_angle(relativeposition);
	log_this(s, "[%s] Angle absolute to destination (trigonometric wise) to turn %d...\n", __FILE__, absoluteangle);
	//log_this(s, "[%s] Current angle of the robot %d...\n", __FILE__, s->angle);
	int relativeangle;
	if(s->angle>=0) relativeangle=absoluteangle-s->angle;
	else relativeangle=absoluteangle+s->angle;
    log_this(s, "[%s]Relative angle in trigonometric way to turn %d...\n", __FILE__, relativeangle);
    //need to be clockwise for the turn function so send - relative angle to the function
	turn(s, TURNING_SPEED, -relativeangle);

	update_angle(s,absoluteangle);
	go_straight(s, MAX_WHEEL_SPEED, distancetodest);
	update_pos(s, desiredposition);
	return 0;
}


//TODO check if it turns the right way
/**
* Turn from a given angle at a given speed
* @param s State of LeE
* @param speed Turning speed of LeE
* @param angle Angle to turn in degrees and clockwise
* @return 0 if eveything is alright, 1 if the angle is too small for the gyro
*/
int turn(state *s, int speed, int angle){
    if (abs(angle)<ERROR_MARGIN){
        log_this(s, "[%s] : The angle is too small (%d). Not turning.\n", __FILE__, angle);
        return 1;
    }
    log_this(s, "[%s] : Turning from %d degrees.\n", __FILE__, angle );
    int angle_sign = sign(angle);
    speed = speed * angle_sign;
    ev3_update_sensor_val(s->gyro);     // We make sure that the gyro is updated
    int current_angle = s->gyro->val_data[0].s32 ; // We use the raw data of the gyro, not the clean one
    int goal = current_angle + angle;
    ev3_set_speed_sp(s->leftmotor, speed);
    ev3_set_speed_sp(s->rightmotor, -speed);
    command_wheels(s, RUN_FOREVER);
    // While we haven't reach the desired angle, we keep turning in the right direction
    while(angle_sign * current_angle < angle_sign * goal){
        ev3_update_sensor_val(s->gyro);
        current_angle = s->gyro->val_data[0].s32 ;
    }
    command_wheels(s, STOP);
    return 0;
}

/**
 * Function to correct the position while moving
 * @param s State of LeE
 * @return the difference between the actual angle and the wanted angle, 0 if this difference is smaller than the error margin
 */
int is_running_in_correct_angle(state *s){
	int actualangle = gyro_angle(s);
	//+- ERROR_M degrees is ok
    int angle_diff = clean_angle(s->angle - actualangle);
    printf("Is running: %d\t%d\n", actualangle, angle_diff);
	log_this(s, "[%s] Angle diff is %d\n", __FILE__, angle_diff);
	if(abs(angle_diff) > ERROR_MARGIN){
		return angle_diff;
	}
	return 0;
}


//Same functions but using the compass, To test and choose/combine.

/**
* Turn from a given angle at a given speed using the compass
* @param s State of LeE
* @param speed Turning speed of LeE
* @param angle Angle to turn in degrees and clockwise
* @return 0 if eveything is alright, 1 if the angle is too small for the gyro
*/
int turn_compass(state *s, int speed, int angle){
    if (abs(angle)<ERROR_MARGIN){
        log_this(s, "[%s] : The angle is too small (%d). Not turning.\n", __FILE__, angle);
        return 1;
    }
    log_this(s, "[%s] : Turning from %d degrees.\n", __FILE__, angle );
    int angle_sign = sign(angle);
    speed = speed * angle_sign;
    ev3_update_sensor_val(s->compass);     // We make sure that the compass is updated
    int current_angle = s->compass->val_data[0].s32 ; // We use the raw data of the compass, not the clean one
    int goal = current_angle + angle;
    ev3_set_speed_sp(s->leftmotor, speed);
    ev3_set_speed_sp(s->rightmotor, -speed);
    command_wheels(s, RUN_FOREVER);
    // While we haven't reach the desired angle, we keep turning in the right direction
    while(angle_sign * current_angle < angle_sign * goal){
        ev3_update_sensor_val(s->compass);
        current_angle = s->compass->val_data[0].s32 ;
    }
    command_wheels(s, STOP);
    return 0;
}


/**
 * Function to correct the position while moving
 * @param s State of LeE
 * @return the difference between the actual angle and the wanted angle, 0 if this difference is smaller than the error margin
 */
int is_running_in_correct_angle_compass(state *s){
	int actualangle = compass_angle(s);
	//+- ERROR_M degrees is ok
    int angle_diff = clean_angle(s->angle - actualangle);
    printf("Is running: %d\t%d\n", actualangle, angle_diff);
	log_this(s, "[%s] Angle diff is %d\n", __FILE__, angle_diff);
	if(abs(angle_diff) > ERROR_MARGIN){
		return angle_diff;
	}
	return 0;
}

/**
 * Go straight for a distance in cm
 * @param  s        State structure
 * @param  speed    Speed for  the LeE (usually MAX_WHEEL_SPEED)
 * @param  distance Distance in cm
 * @return          0 if everything is allright
 */
int go_straight_compass(state *s, int speed, int distance){
    log_this(s, "[%s] : Going straigth for%d cm\n", __FILE__, distance);
    s->angle = compass_angle(s);

    // We divide the wanted distance in steps od STEPLENGTH
	int nb_of_steps = distance / STEPLENGTH;
    int remaining_distance = distance % STEPLENGTH;
	printf("STP: %d \t RD: %d\n", nb_of_steps, remaining_distance);
    int i;
    // After each step we correct the direction of the robot
    for (i=0; i<nb_of_steps; i++){
        wheels_run_distance(s, speed, STEPLENGTH);
        turn_compass(s, TURNING_SPEED, is_running_in_correct_angle_compass(s));
    }
    // We then go for the remaining distance and correct the angle again
    wheels_run_distance(s, speed, remaining_distance);
    turn_compass(s, TURNING_SPEED, is_running_in_correct_angle_compass(s));
    return 0;
}
