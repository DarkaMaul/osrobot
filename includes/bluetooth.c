#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "logger.h"

void read_from_server(state *s)
{
    char buffer[MSG_MAX_LEN];
    
    int readedBytes = read(s->sock, buffer, MSG_MAX_LEN);
    if (readedBytes < HEADER_SIZE)
    {
        
    }
}
