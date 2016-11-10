#ifndef __CONFIG_H__
#define __CONFIG_H__

//Logger configuration
#define LOGFILE "logs/log.txt"

//Robot constants

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

#define HEADER_SIZE 5
#define MSG_MAX_LEN 60 //58?

#define SERV_ADDR ""
#define TEST_SERV_ADDR "127.0.0.1"

#endif
