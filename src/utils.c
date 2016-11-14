#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <arpa/inet.h>
#include <stdarg.h>

#include "main.h"
#include "config.h"
#include "logger.h"

/**
 * Initialize the bluetooth connexion to the SERV_ADDR (defined in config)
 * @todo TEST
 * @return [description]
 */
int init_bluetooth()
{
    struct sockaddr_rc addr = {0};
    int status, s;

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    //str2ba(SERV_ADDR, &addr.rc_bdaddr);

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    if (status == 0)
        return s;
    else
        return -1;

    return 0;
}

/**
 * Init the inet connexion
 * @param  s State structure
 * @return   Socket ID or -1 if there is an error.
 */
int init_inet(state *s)
{
    struct sockaddr_in addr = {0};
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* bind INET socket */
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(TEST_SERV_ADDR);
    addr.sin_port = htons(8888);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        log_this(s, "[Utils] Can't connect to INET!");
        return -1;
    }

    return sock;
}

/**
 * Close the inet socket previously opened
 * @param s State structure
 */
void close_inet(state *s)
{
    close(s->sock);
    log_this(s, "[Utils] Closing socket INET.");
}

/**
 * Convert an uint16 to byte in little endian
 * @param id     Unsigned int to convert
 * @param bytes Char* to hold the result (bytes+1 is the most significant byte)
 */
void uint_to_bytes(uint16_t id, char bytes[2])
{
    bytes[1] = (id >> 8) & 0xff;
    bytes[0] = id & 0xff;
}

/**
 * Send a message to the destination
 * @param  s           State structure
 * @param  messageType Message type (must be in MSG_* constants)
 * @param  destination Address of the reciever (format 0xXX)
 * @param  VARARGS     Param for the message type:
 *                          ACK : (unsigned int) id of the message to ack
 *                                (int) status of the ACK (ACK_OK || ACK_NOK)
 *                          NEXT: void
 *                          CUSTOM: not implemeented
 *                          POSITION: (int) x
 *                                    (int) y
 *
 * @return             0 if everything is OK || -1 if not
 */
int send_message(state *s, int messageType, int destination, ...)
{

    va_list argumentList;
    va_start(argumentList, destination);

    char message[MSG_MAX_LEN];
    int messageLength = 0;
    char bytes[2];


    uint16_t messageId = s->msgId++;
    uint_to_bytes(messageId, bytes);

    message[HEADER_ID0] = bytes[0];
    message[HEADER_ID1] = bytes[1];
    message[HEADER_SRC] = TEAM_ID;
    message[HEADER_DEST] = destination;
    message[HEADER_TYPE] = messageType;

    unsigned int ackId;
    int statusCode;
    uint16_t position[2];

    switch(messageType)
    {
        //Send ACK message
        // char* id of the message to acknowledge
        // int ack type
        case MSG_ACK:
            ackId = va_arg(argumentList, int);
            statusCode = va_arg(argumentList, int);

            uint_to_bytes((uint16_t) ackId, bytes);

            message[5] = bytes[0];
            message[6] = bytes[1];
            message[7] = statusCode;

            messageLength = MSG_ACK_LEN;
            break;

        case MSG_NEXT:
            messageLength = MSG_NEXT_LEN;
            break;

        case MSG_CUSTOM:
            log_this(s, "[Utils] No custom messages implemeented yet.\n");
            break;

        case MSG_POSITION:
            position[0] = (uint16_t) va_arg(argumentList, int);
            position[1] = (uint16_t) va_arg(argumentList, int);

            *((uint16_t *) (message+5)) = position[0];
            *((uint16_t *) (message+7)) = position[1];

            messageLength = MSG_POSITION_LEN;
            break;

        default:
            log_this(s, "[Utils] Unknown message type to send (%d).\n", messageType);
            return -1;
    }

    va_end(argumentList);

    //Send the message
    write(s->sock, message, messageLength);

    //write(STDOUT_FILENO, message, messageLength);
    printf("%x %x\n", bytes[0], bytes[1]);
    log_this(s, "[Utils] Message of type %d with id %u sended to %d\n", messageType, s->msgId, destination);

    return 0;
}

/**
 * Read a message from the server
 * 
 * @param  s      State structure
 * @param  buffer Where to store the readed bytes
 * @return char   Message type
 * @todo !!!
 */
char read_from_server(state *s, char *buffer)
{
    int readedBytes = read(s->sock, buffer, MSG_MAX_LEN);

    log_this(s, "[Utils] Waiting for input from the server.\n");

    if (readedBytes <= 0)
    {
        log_this(s, "[Utils] Server closed the connexion unexpectedly (readedBytes: %d).\n", readedBytes);
        return -1;
    }
/*
    if (buffer[HEADER_DEST] != (TEAM_ID & 0xFF))
    {
        log_this(s, "[Utils] Recieved a message for an other team (%d)\n", (int) buffer[HEADER_DEST]);
        return -2;
    }

    if (buffer[HEADER_SRC] != SERVER_ID)
    {
        log_this(s, "[Utils] Recieved a message something that is not the server (%d)! \n", buffer[HEADER_SRC]);
        return -2;
    }

    if (readedBytes < HEADER_SIZE)
    {
        log_this(s, "[Utils] Message malformed (header size is less %d/5)! \n", readedBytes);
        return -2;
    }
    */

    int expectedMessageLength;
    switch (buffer[HEADER_TYPE])
    {
        case MSG_START:
            expectedMessageLength = 7;
            break;

        case MSG_STOP:
            expectedMessageLength = 5;
            break;

        case MSG_CUSTOM:
            expectedMessageLength = MSG_MAX_LEN;
            break;

        case MSG_KICK:
            expectedMessageLength = 6;
            break;

        default:
            log_this(s, "[Utils] Unknow message type (%c)! \n, ", buffer[HEADER_TYPE]);
            return -2;
    }

    if (readedBytes < expectedMessageLength && buffer[HEADER_TYPE] != MSG_CUSTOM)
    {
        log_this(s, "[Utils] Message malformed (type was %d (expect %d) and size was %d).\n", buffer[HEADER_TYPE], expectedMessageLength, readedBytes);
        return -2;
    }

    return buffer[HEADER_TYPE];
}
