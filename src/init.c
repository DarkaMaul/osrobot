#include <signal.h>
#include <pthread.h>

#include "init.h"
#include "main.h"
#include "sensors.h"
#include "logger.h"
#include "motors.h"
#include "utils.h"
#include "threads.h"

extern state* s;

void signal_handler(int signalNumber)
{
    if (signalNumber == SIGINT)
    {
        log_this(s, "[Init] Recvieved SIGINT.\n");
        nice_exit(s, EXIT_FAILURE);
    }
}

/**
 * Initialize LeE
 * @param  s Structure state
 * @return   0 in case of success || 1 in case of error
 */
int init_robot(state *s, mainpos *p )
{
    int returnValue;
    //Initialize the logger
    init_logger(s);

    //Initialize sockets
    /*s->sock = init_inet(s);
    if (s->sock == -1)
        nice_exit(s, EXIT_FAILURE);
    */

    //Init the motors
    init_motors(s);

    //Init sensors
    returnValue = 0;// init_sensors(s);
    if (returnValue !=  0)
    {
        log_this(s, "[%s] Sensors unable to initialize.\n", __FILE__);
        return 1;
    }

    //Init position
    init_pos(s);
/*
    //Init Threads
    if(pthread_create(&(s->threadPosition), NULL, (void *) position_thread, (void*) &s))
    {
        log_this(s, "[%s] Unable to create position thread\n", __FILE__);
        nice_exit(s, EXIT_FAILURE);
    }
*/
    return 0;
}



/**
 * Nice exit the robot
 * @param s         Structure state
 * @param exitState EXIT_SUCCESS || EXIT_FAILURE
 */
void nice_exit(state *s, int exitState)
{
    log_this(s, "[Main] Nice exit called.\n");

    if (exitState == EXIT_SUCCESS)
        close_threads(s);

    //TODO @CLOSE BLUETOOTH
    if (s->sock > 0)
        close_inet(s);

    if (s->logfile_fd > 0)
        close_logger(s);

    ev3_delete_motors(s->motors);
    exit(exitState);
}
