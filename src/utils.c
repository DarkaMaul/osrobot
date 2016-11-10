#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <arpa/inet.h>

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
