#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"
#include "main.h"
#include "config.h"

/**
 * @fn void init_logger (state *s)
 * @brief Initialize the logger.
 * @exception Unable to open a file in the system
*/
void init_logger(state *s)
{
	FILE* fp = fopen(LOGFILE, "w");
    if (fp == NULL)
    {
        perror("[Logger] Unable to open log file.\n");
        exit(1);
    }

    s->logfile_fd = fp;

    setvbuf(s->logfile_fd, NULL, _IONBF, 0);
    log_this(s, "[Logger] Initialize logger for this session.\n");
}

/**
 * @fn void close_logger (state *s)
 * @brief Close the logger.
*/
void close_logger(state *s)
{
    log_this(s, "[Logger] Close logger.\n");
    fclose(s->logfile_fd);
}

/**
 * @fn void log_this (state *s, const char *format, ...)
 * @brief Log the printf(-formated string into the log file.
 *
 * This function is used exactly like printf(:
 * log_this(s, "[FileName] Error with the rotation of motor %d\n", motor_id);
 *
 * @todo Add automatically the \n.
*/
void log_this(state *s, const char *format, ...)
{
    va_list args;
    pthread_mutex_lock(&(s->mutexLogFile));
    va_start(args, format);
    vfprintf(s->logfile_fd, format, args);
    //vprintf(format,args); //DELETED BC IT MESSES WITH THREADS
    va_end(args);
    fflush(s->logfile_fd);
    pthread_mutex_unlock(&(s->mutexLogFile));
}
