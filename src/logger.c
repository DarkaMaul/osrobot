/**
 * @file logger.c
 * @date 7 Nov 2016
 * @brief Logger system
 */

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
 * @brief Log the printf-formated string into the log file.
 *
 * This function is used exactly like printf:
 * log_this(s, "[FileName] Error with the rotation of motor %d\n", motor_id);
 *
 * @todo Add automatically the \n.
*/
void log_this(state *s, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(s->logfile_fd, format, args);
    va_end(args);
}
