#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "logger.h"
#include "ev3c.h"

state st;
state* s = &st;

int main(int argc, char *argv[])
{
	//Start logger	
	init_logger(s);	

	//Initialize the robot
	//Capteurs Moteur "Connexion"

	//Initialize the "trip"
	
	//ACTION
	
	//Reset robot
	
	//Close logger
	close_logger(s);
	
	return 0;
}

