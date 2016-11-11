#include <stdio.h>
#include <unistd.h>

#include "ev3c.h"
#include "tester.h"

/**
 * Theoretical max speed 1560 counts/s for the small motor
 */
/*
int grab(int absoluteposition, int speed)
{
	ev3_motor_ptr a;
	ev3_motor_ptr motors = ev3_load_motors();

	FILE* fp = fopen("logs/motortest.txt", "w");

	a = ev3_search_motor_by_port(motors, 'A');
	ev3_open_motor(a);
	for(int i=0;i<2;i++){
		if(i==1) {
			speed=-speed;
			absoluteposition=-absoluteposition;
		}
		ev3_reset_motor(a);
		ev3_set_speed_sp( a, -1*speed );
		ev3_set_position_sp(a, absoluteposition);
		ev3_command_motor_by_name(a, "run-to-abs-pos");

		while (ev3_motor_state(a) & MOTOR_RUNNING);
	}
	fprintf(fp, "Motor opened\n");



	ev3_delete_motors(motors);

	fclose(fp);

	return 0;
}


*
 * Theoretical max speed 1560 counts/s for the small motor

int release(int absoluteposition, int speed)
{
	ev3_motor_ptr a;
	ev3_motor_ptr motors = ev3_load_motors();

	FILE* fp = fopen("logs/motortest.txt", "w");

	a = ev3_search_motor_by_port(motors, 'A');
	ev3_open_motor(a);
	for(int i=0;i<2;i++){
		if(i==1) {
			speed=-speed;
			absoluteposition=-absoluteposition;
		}
		ev3_reset_motor(a);
		ev3_set_speed_sp( a, -1*speed );
		ev3_set_position_sp(a, absoluteposition);
		ev3_command_motor_by_name(a, "run-to-abs-pos");

		while (ev3_motor_state(a) & MOTOR_RUNNING);
	}
	fprintf(fp, "Motor opened\n");



	ev3_delete_motors(motors);

	fclose(fp);

	return 0;
}

int testRobot()
{
	ev3_motor_ptr motors = ev3_load_motors();
	ev3_motor_ptr motor = motors;

	FILE* fp = fopen("logs/motortest.txt", "w");

	while (motor)
	{
		ev3_reset_motor(motor);

		ev3_open_motor(motor);
		ev3_set_speed_sp( motor, -500 );
		ev3_command_motor_by_name(motor, "run-forever");
		motor = motor->next;
		fprintf(fp, "Motor opened\n");
	}
	sleep(3);

	ev3_delete_motors(motors);

	fclose(fp);

	return 0;
}
*/




