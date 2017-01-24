#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <math.h>

#include "main.h"
#include "logger.h"
#include "ev3c.h"
#include "utils.h"
#include "motors.h"
#include "init.h"
#include "robot.h"
#include "sensors.h"
#include "first_runner.h"
#include "tester.h"
#include "config.h"

state st;
state* s = &st;

mainpos mpos;
mainpos* p = &mpos;

/**
 * Game function : wait until the server send  a start message and process
 * @return void
 */
void game()
{
    //Game function o_OOOOO_o

    char buf[100];
    int returnValue;
    while(1)
    {
        returnValue  = read_message_from_server(s, buf);
        if (returnValue == -1)
            continue;

        if (buf[HEADER_TYPE] == MSG_START)
            break;
    }

    load_game_params(s, buf);
    game_wrapper(s, p);

    return;
}

int main(int argc, char** argv)
{
    signal(SIGINT, signal_handler);

    init_robot(s);

    game();

    nice_exit(s, EXIT_SUCCESS);

    return 0;
}
