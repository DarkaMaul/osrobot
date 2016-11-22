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
 * Read byutes from server
 * @param  s      State structure
 * @param  buffer Where to store the readed bytes
 * @return        Number of bytes read
 */
int read_from_server(state *s, char *buffer)
{
    int readedBytes = read(s->sock, buffer, MSG_MAX_LEN);

    if (readedBytes <= 0)
    {
        log_this(s, "[Utils] Impossible to read from server (error code: %d)", readedBytes);
        return -1;
    }

    return readedBytes;
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

    unsigned int messageId = s->msgId++;
    *((uint16_t *) message) = messageId;
    message[2] = TEAM_ID;
    message[3] = destination;
    message[4] = messageType;

    unsigned int ackId;
    int statusCode;
    uint16_t position[2];

    switch(messageType)
    {
        //Send ACK message
        // char* id of the message to acknowledge
        // int ack type
        case MSG_ACK:
            ackId = va_arg(argumentList, unsigned int);
            statusCode = va_arg(argumentList, int);

            *((uint16_t *) (message+5)) = ackId;
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

    log_this(s, "[Utils] Message of type %d with id %u sended to %d\n", messageType, s->msgId, destination);

    return 0;
}
