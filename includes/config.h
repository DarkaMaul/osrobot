#ifndef __CONFIG_H__
#define __CONFIG_H__

//Logger configuration
#define LOGFILE "logs/log.txt"

//Team constants
#define TEAM_ID 1
#define SERVER_ID 0XFF

//Robot constants

//Ports of motors
#define PORT_LEFT_MOTOR "D"
#define PORT_RIGHT_MOTOR "B"
#define PORT_GRAB_MOTOR "A"
#define PORT_SWEEP_MOTOR "D"

//Grabing constants
#define GRAB_POSITION 18
#define MAX_GRABBING_SPEED 1050
#define INIT_GRAB_POSITION 0


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
#define MSG_MAX_LEN 60 //58?

#define SERV_ADDR ""
#define TEST_SERV_ADDR "127.0.0.1"

#endif
