#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

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

//#include "config.h"

state st;
state* s = &st;

void game()
{
    //Game function o_OOOOO_o

    //Let's wait for the starting message
    
}

int main(int argc, char *argv[])
{
    //Register signal handler
    signal(SIGINT, signal_handler);

    //Init the LeE
    init_robot(s);
    init_inet(s);
    //Close external ressources

    nice_exit(s, EXIT_SUCCESS);
	return 0;
}
