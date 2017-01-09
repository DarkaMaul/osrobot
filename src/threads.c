#include <pthread.h>

#include "main.h"
#include "config.h"
#include "logger.h"
#include "utils.h"

/**
 * Function to handle the position thread
 * @param voidS Void *  but actually a pointer to state structure
 * @return void
 */
void position_thread(void *voidS)
{
    state *s = (state *) voidS;

    log_this(s, "[%s] Waiting for server\n");
    while(1)
    {
        pthread_mutex_lock(&(s->mutexGameStarted));
        if (s->gameStarted == TRAVELLING)
        {
            pthread_mutex_unlock(&(s->mutexGameStarted));
            break;
        }

        pthread_mutex_unlock(&(s->mutexGameStarted));
        sleep(2);
    }

    log_this(s, "[%s] Starting to send position\n", __FILE__);

    position currentPos;
    int i = 0;
    while(1)
    {
        pthread_mutex_lock(&(s->mutexPosition));
        currentPos = s->curPos;
        pthread_mutex_unlock(&(s->mutexPosition));

        pthread_mutex_lock(&(s->mutexSockUsage));
        send_position(s, currentPos);
        pthread_mutex_unlock(&(s->mutexSockUsage));

        if (++i == 8)
        {
            pthread_mutex_lock(&(s->mutexGameStarted));
            if (s->gameStarted == IMMOBILE)
            {
                pthread_mutex_unlock(&(s->mutexGameStarted));
                break;
            }
            pthread_mutex_unlock(&(s->mutexGameStarted));
            i = 0;
        }
        sleep(2);
    }

    log_this(s, "[%s] End thread position.\n", __FILE__);

    return;
}

/**
 * Close the threads if they are open when we try to exit
 * @param  s State structure
 * @return   0
 */
int close_threads(state *s)
{
    if (s->threadPosition != 0)
        pthread_join(s->threadPosition, NULL);

    return 0;
}
