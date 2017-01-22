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
int init_robot(state *s)
{
    int returnValue;
    //Init the locks
    init_locks(s);

    //Initialize the logger
    init_logger(s);

    //Initialize sockets
    /*s->sock = init_bluetooth();
    if (s->sock == -1)
        nice_exit(s, EXIT_FAILURE);
    */

    //Init the motors
    init_motors(s);

    //Init sensors
    returnValue = init_sensors(s);
    if (returnValue !=  0)
    {
        log_this(s, "[%s] Sensors unable to initialize.\n", __FILE__);
        return 1;
    }

    //Init position
    init_pos(s);
/*
    //Set game started
    pthread_mutex_lock(&(s->mutexGameStarted));
    s->gameStarted = IMMOBILE;
    pthread_mutex_unlock(&(s->mutexGameStarted));

    //Init Threads
    if(pthread_create(&(s->threadPosition), NULL, (void *) position_thread, NULL))
    {
        log_this(s, "[%s] Unable to create position thread\n", __FILE__);
        nice_exit(s, EXIT_FAILURE);
    }
*/
    return 0;
}

int init_locks(state *s)
{
    int rc;
    if ((rc = pthread_mutex_init(&(s->mutexLogFile), NULL)) != 0)
        return -1;

    //printf("RC:%d\n", rc);

    if (pthread_mutex_init(&(s->mutexGameStarted), NULL) != 0)
        return -1;

    if (pthread_mutex_init(&(s->mutexSockUsage), NULL) != 0)
        return -1;

    if (pthread_mutex_init(&(s->mutexPosition), NULL) != 0)
        return -1;

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

    if (s->sock > 0)
        close_socket(s);

    if (s->logfile_fd > 0)
        close_logger(s);

    ev3_delete_motors(s->motors);
    exit(exitState);
}
