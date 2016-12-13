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
    int readedBytes = read(s->sock, buffer, (size_t) MSG_MAX_LEN);

    if (readedBytes <= 0)
    {
        log_this(s, "[Utils] Impossible to read from server (error code: %d)", readedBytes);
        return -1;
    }

    return readedBytes;
}

int read_message_from_server(state *s, char *buffer)
{
    int readedBytes = read_from_server(s, buffer);

    if (readedBytes <= 0)
        return readedBytes;

    // if (buffer[HEADER_SRC] != (char) SERVER_ID && buffer[HEADER_SRC] != (unsigned char) s->ally + 1)
    // {
    //     log_this(s, "[%s] Message not coming from server.\n", __FILE__);
    //     return -1;
    // }

    if (buffer[HEADER_DEST] != (char) TEAM_ID)
    {
        log_this(s, "[%s] Message recieved but not for me.\n", __FILE__);
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
int send_message(state *s, int messageType, unsigned char destination, ...)
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
            position[0] = (int16_t) va_arg(argumentList, int);
            position[1] = (int16_t) va_arg(argumentList, int);

            *((int16_t *) (message+5)) = position[0];
            *((int16_t *) (message+7)) = position[1];

            messageLength = MSG_POSITION_LEN;
            break;

        case MSG_BALL:
            message[5] = va_arg(argumentList, int);
            position[0] = (int16_t) va_arg(argumentList, int);
            position[1] = (int16_t) va_arg(argumentList, int);

            *((int16_t *) (message+6)) = position[0];
            *((int16_t *) (message+8)) = position[1];
            messageLength = MSG_BALL_LEN;
            printf("We did it boys \n");
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

/**
 * Send the position of LeE
 * @param  s State structure
 * @return   0 if everythingis OK || -1 if not
 */
int send_position(state *s)
{
    return send_message(s, MSG_POSITION, SERVER_ID, s->curPos.x, s->curPos.y);
}

/**
 * Parse the START message to load the game parameters
 * @param  s      State structure
 * @param  buffer Raw message
 * @return        0 in case of success | -1 otherwise
 */
int load_game_params(state *s, char *buffer)
{
    s->role = -1;
    s->side = -1;
    s->ally = -1;

    if ((unsigned char) buffer[5] == ROLE_FIRST || (unsigned char) buffer[5] == ROLE_SECOND)
        s->role = (unsigned char) buffer[5];

    if ((unsigned char) buffer[6] == SIDE_RIGHT || (unsigned char) buffer[6] == SIDE_LEFT)
        s->side = (unsigned char) buffer[6];

    //TODO
    if ((unsigned char) buffer[7] >= 1 && (unsigned char) buffer[7] < 254)
        s->ally = (unsigned char) buffer[7];

    if (s->role == 255 || s->side == 255 || s->ally == 255)
    {
        log_this(s, "[Utils] Error while defining game constants in load_game_params(%u, %u, %u).\n", (unsigned char) buffer[5], (unsigned char) buffer[6], (unsigned char) buffer[7]);
        return -1;
    }

    return 0;
}

/**
 * Check if in arena
 * @param  s              Structure state
 * @param  testedPosition Tested position
 * @return                bool
 */
int is_in_arena(state *s, position testedPosition)
{
    if (s->type == SMALL_ARENA)
    {
        if(testedPosition.x > 0 && testedPosition.x < SMALL_ARENA_MAX_X
           && testedPosition.y > 0 && testedPosition.y < SMALL_ARENA_MAX_Y)
           return 0;

    } else if (s->type == BIG_ARENA)
    {
        if (testedPosition.y > 0 && testedPosition.y < BIG_ARENA_MAX_Y)
        {
            if ((s->side == SIDE_RIGHT && testedPosition.x > 0 && testedPosition.x < BIG_ARENA_MAX_X)
               || (s->side == SIDE_LEFT && testedPosition.x > -BIG_ARENA_MAX_X && testedPosition.x < 0))
               return 0;
        }
    }

    return -1;
}
