#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "utils.h"

state st;
state* s = &st;

int obstacle_detected;
int movement_completed;
int ack_arrived;

void sigint_handler(int sig_n) {
	if (sig_n == SIGINT) {
		//stop the motors
       		ev3_command_motor_by_name(s->m_dx, "stop");
      		ev3_command_motor_by_name(s->m_sx, "stop");

		ev3_set_led(LEFT_LED, GREEN_LED, 0);
		ev3_set_led(LEFT_LED, RED_LED, 0);

		if (s->gripper_state == DOWN){
			close_gripper(s);
		}

		else if (s->gripper_state == UP){
			open_gripper(s);
			close_gripper(s);
		}
		

		//Let's delete the list in the very end. It will also close the
		//sensors
		ev3_delete_sensors(s->sensors);
		
		//Let's delete the list in the very end. It will also close the
		//motors
		ev3_delete_motors(s->motors);

		// Close the log file
		fclose(s->logfile_fd);
		exit(0);
	}
}


int main(int argc, char *argv[]) {

	//Registering the SIGINT handler
	signal(SIGINT, sigint_handler);

	// loads and set all the motors, all the sensors and useful leds
	load_and_set(s);

	wait_and_read_start(s);

	s->x = INITIAL_X;
	s->y = INITIAL_Y;
	s->angle = 0;

	if (s->rank > 0) {
		log_printf(s, "[main] Starting follower\n");
		follower();
	}

	if (s->rank == 0) {
		log_printf(s, "[main] Starting leader\n");
		leader();
	}

	log_printf(s, "\n\n\n[main] Bye bye! :)\n\n\n");
	fclose(s->logfile_fd);

	return 0;
}

