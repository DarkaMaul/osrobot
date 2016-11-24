#ifndef __CONFIG_H__
#define __CONFIG_H__

//Logger configuration
#define LOGFILE "logs/log.txt"

//Team constants
#define TEAM_ID 1
#define SERVER_ID 255
#define SERV_ADDR ""
#define TEST_SERV_ADDR "127.0.0.1"

//Robot constants
#define ERROR_MARGIN 5

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
#define GRAB_POSITION 18
#define MAX_GRABBING_SPEED 1050
#define INIT_GRAB_POSITION 0

//Wheel motors constants
#define MAX_WHEEL_SPEED 1050
#define WHEEL_DIAMETER 5.6

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

#define MSG_ACK_LEN 8
#define MSG_NEXT_LEN 5
#define MSG_POSITION_LEN 9

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

//Ball constants
#define BALL_COLOR_1 5
#define BALL_COLOR_2 2

#define US_MAX_DISTANCE 1500//@TODO
#define US_MIN_DISTANCE 150//@TODO
#define US_MAX_RETURN_VAL 2550

#endif
