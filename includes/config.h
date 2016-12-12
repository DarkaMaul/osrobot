#ifndef __CONFIG_H__
#define __CONFIG_H__

//Logger configuration
#define LOGFILE "logs/log.txt"

//Team constants
#define TEAM_ID 1
#define SERVER_ID -1
#define SERV_ADDR ""
#define TEST_SERV_ADDR "127.0.0.1"

//Robot constants
#define ERROR_MARGIN 2
#define ERROR_DISTANCE_MARGIN 5
#define WHEELS_TO_END 11

//Motors commands
#define RUN_FOREVER 0
#define RUN_TO_ABS_POS 1
#define RUN_TO_REL_POS 2
#define RUN_TIMED 3
#define RUN_DIRECT 4
#define STOP 5
#define RESET 6

//Motors stop actions
#define COAST 0
#define BRAKE 1
#define HOLD 2

//Ports of motors
#define PORT_LEFT_MOTOR "D"
#define PORT_RIGHT_MOTOR "A"
#define PORT_GRAB_MOTOR "C"
#define PORT_SWEEP_MOTOR "B"

//Sensor's port
#define PORT_SENSOR_COLOR 4
#define PORT_SENSOR_GYRO 2
#define PORT_SENSOR_COMPASS 1
#define PORT_SENSOR_SONAR 3

//Grabing constants
#define GRAB_POSITION 23
#define MAX_GRABBING_SPEED 550
#define RELEASING_SPEED 100
#define INIT_GRAB_POSITION -15

//Wheel motors constants
#define MAX_WHEEL_SPEED 1050
#define WHEEL_DIAMETER 5.6
#define STEPLENGTH 60
#define TURNING_SPEED 30

//Positions of of fixed elements
#define CENTER_POSITION 0
#define MAX_WIDTH_POSITION 120
#define MAX_HEIGHT_POSITION 400

//Bluetooth
#define MSG_ACK 0
#define MSG_NEXT 1
#define MSG_START 2
#define MSG_STOP 3
#define MSG_CUSTOM 4
#define MSG_KICK 5
#define MSG_POSITION 6
#define MSG_BALL 7

#define PICK_BALL 0
#define LET_BALL  1

#define MSG_ACK_LEN 8
#define MSG_NEXT_LEN 5
#define MSG_POSITION_LEN 9
#define MSG_BALL_LEN 10;

#define ACK_OK 0
#define ACK_NOK 1
#define HEADER_SIZE 5
#define MSG_MAX_LEN 58 //58?

#define HEADER_ID0  0
#define HEADER_ID1  1
#define HEADER_SRC  2
#define HEADER_DEST 3
#define HEADER_TYPE 4

#define NOT_FOR_ME -3

//Game constants
#define ROLE_FIRST 0
#define ROLE_SECOND 1
#define SIDE_RIGHT 0
#define SIDE_LEFT 1

//Field config
#define SMALL_ARENA 0
#define BIG_ARENA 1

#define SMALL_ARENA_MAX_X 120
#define SMALL_ARENA_MAX_Y 200

#define BIG_ARENA_MAX_X 120
#define BIG_ARENA_MAX_Y 400

//Small_FirstRunner_Startingposition_Point0_X
#define S_FR_S_0_X 70
#define S_FR_S_0_Y 10
#define S_FR_S_1_X 70
#define S_FR_S_1_Y 50
#define S_FR_S_2_X 110
#define S_FR_S_2_Y 50
#define S_FR_S_3_X 110
#define S_FR_S_3_Y 10

//Small_BallArena_Point0_X
#define S_BA_0_X 50
#define S_BA_0_Y 90
#define S_BA_1_X 50
#define S_BA_1_Y 110
#define S_BA_2_X 70
#define S_BA_2_Y 110
#define S_BA_3_X 70
#define S_BA_3_Y 90

//Small_SecondRunner_EndPosition_Point0_X
#define S_SR_E_0_X 10
#define S_SR_E_0_Y 10
#define S_SR_E_1_X 10
#define S_SR_E_1_Y 50
#define S_SR_E_2_X 50
#define S_SR_E_2_Y 50
#define S_SR_E_3_X 50
#define S_SR_E_3_Y 10

//Small_SecondRunner_Startingposition_Point0_X
#define S_SR_S_0_X 10
#define S_SR_S_0_Y 150
#define S_SR_S_1_X 10
#define S_SR_S_1_Y 190
#define S_SR_S_2_X 50
#define S_SR_S_2_Y 190
#define S_SR_S_3_X 50
#define S_SR_S_3_Y 150

//Small_FirstRunner_Endingposition_Point0_X
#define S_FR_E_0_X 70
#define S_FR_E_0_Y 150
#define S_FR_E_1_X 70
#define S_FR_E_1_Y 190
#define S_FR_E_2_X 110
#define S_FR_E_2_Y 190
#define S_FR_E_3_X 110
#define S_FR_E_3_Y 150

// The following coordinates are for the right side of the arena. For the Left side, just invert the X's.
#define OBSTACLE_WIDTH 15

//Large_Obstacle1(first one)_Point0_X 
#define L_O1_0_X 0 
#define L_O1_0_Y 150
#define L_O1_1_X 0
#define L_O1_1_Y 165
#define L_O1_2_X 60
#define L_O1_2_Y 165
#define L_O1_3_X 60
#define L_O1_3_Y 150
#define L_O2_0_X 60
#define L_O2_0_Y 250
#define L_O2_1_X 60
#define L_O2_1_Y 265
#define L_O2_2_X 120
#define L_O2_2_Y 265
#define L_O2_3_X 120
#define L_O2_3_Y 250

//Large_Ballarea_Point0_X
#define L_BA_0_X 50
#define L_BA_0_Y 190
#define L_BA_1_X 50
#define L_BA_1_Y 210
#define L_BA_2_X 70
#define L_BA_2_Y 210
#define L_BA_3_X 70
#define L_BA_3_Y 190

//Large_FirstRunner_Startingposition_Point0_X
#define L_FR_S_0_X 10
#define L_FR_S_0_Y 10
#define L_FR_S_1_X 10
#define L_FR_S_1_Y 50
#define L_FR_S_2_X 50
#define L_FR_S_2_Y 50
#define L_FR_S_3_X 50
#define L_FR_S_3_Y 10

//Large_FirstRunner_Endingposition_Point0_X
#define L_FR_E_0_X 10
#define L_FR_E_0_Y 350
#define L_FR_E_1_X 10
#define L_FR_E_1_Y 390
#define L_FR_E_2_X 50
#define L_FR_E_2_Y 390
#define L_FR_E_3_X 50
#define L_FR_E_3_Y 350


#define BALL_CATCH_DISTANCE 3
#define BALL_FOUND 0
#define BALL_NOT_FOUND -1

//Sweeping
#define MAX_SWEEP_ANGLE 45
#define SWEEP_ANGLE 5

//Ball constants
#define BALL_COLOR_1 5
#define BALL_COLOR_2 2

#define US_MAX_DISTANCE 1500//@TODO
#define US_MIN_DISTANCE 150//@TODO
#define US_MAX_RETURN_VAL 2550

#endif
