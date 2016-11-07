#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "main.h"
#include "config.h"

void init_logger(state *s);

void close_logger(state *s);

void log_this(state *s, const char *format, ...);

#endif
