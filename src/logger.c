#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"
#include "main.h"
#include "config.h"

void init_logger(state *s)
{
	FILE* fp = fopen("logs/log.txt", "w");
    if (fp == NULL)
    {
        perror("[Logger] Unable to open log file.\n");
        exit(1);
    }

    s->logfile_fd = fp;
    
    log_this(s, "[Logger] Initialize logger for this session.\n");
}

void close_logger(state *s)
{
    log_this(s, "[Logger] Close logger.\n");
    fclose(s->logfile_fd);
}

void log_this(state *s, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(s->logfile_fd, format, args);
    va_end(args);

    fflush(NULL);
}
