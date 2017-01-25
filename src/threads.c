#include <pthread.h>

#include "main.h"
#include "config.h"
#include "logger.h"
#include "utils.h"

extern state* s;
/**
 * Function to handle the position thread
 * @param voidS Void *  but actually a pointer to state structure
 * @return void
 */
void position_thread()
{
    //Wait until game start
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

    position currentPos;
    int i = 0;
    while(1)
    {
        pthread_mutex_lock(&(s->mutexPosition));
        currentPos = s->curPos;
        pthread_mutex_unlock(&(s->mutexPosition));

        send_position(s, currentPos);

        //Test regularly if we're still moving
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
