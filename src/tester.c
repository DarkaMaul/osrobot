#include <stdio.h>
#include <unistd.h>

#include "ev3c.h"
#include "tester.h"

int runForever()
{

    return 0;
}

int testRobot()
{
    int i;
    FILE* fp = fopen("logs/motortest.txt", "w");

	//Loading all motors
	ev3_motor_ptr motors = ev3_load_motors();
	//Interating over the motors and printing some interesting data
	ev3_motor_ptr motor = motors;
	while (motor)
	{
		ev3_reset_motor(motor);
		fprintf(fp, "==== %s ====\n",motor->driver_name);
		fprintf(fp, "ident: %i\n",motor->driver_identifier);
		fprintf(fp, "motor: %i\n",motor->motor_nr);
		fprintf(fp, "port: %c\n",motor->port);
		fprintf(fp, "command count: %i\n",motor->command_count);
		for (i = 0; i < motor->command_count; i++)
        {
            fprintf(fp, "command[%i]: %s\n",i,motor->commands[i]);
            if (i == 0)
            {
                ev3_command_motor_by_name(motor, "run-forever");
                sleep(5);
                ev3_stop_command_motor(motor, 5);
            }
        }

		fprintf(fp, "stop_command count: %i\n",motor->stop_command_count);
		for (i = 0; i < motor->stop_command_count; i++)
			fprintf(fp, "stop_command[%i]: %s\n",i,motor->stop_commands[i]);
		fprintf(fp, "stop_command %i is choosen.\n",motor->stop_command);
		//Let's open the motors ;)
		ev3_open_motor(motor);
		motor = motor->next;
	}

	//Lets set the first motor to coast mode and the second one to stop:
	ev3_stop_command_motor_by_name( motors, "coast");
	ev3_stop_command_motor_by_name( motors->next, "brake");

	//Lets set the first motor to 30% and the second to 200 counts/s
	ev3_set_duty_cycle_sp( motors, 50 );
	ev3_set_speed_sp( motors->next, 500 );

	//Lets set the first motor to run for 2 seconds and the
	//second on to run one full rotation (360 degree)
	ev3_set_time_sp( motors, 2000 );
	ev3_set_ramp_up_sp( motors, 500);
	ev3_set_ramp_down_sp( motors, 1000);

	ev3_set_position( motors->next, 0 );
	ev3_set_position_sp( motors->next, 360 );

	ev3_command_motor_by_name( motors, "run-timed");
	ev3_command_motor_by_name( motors->next, "run-to-rel-pos");

	//Waiting for the second motor to get to holding position
	while (
		(ev3_motor_state( motors       ) & MOTOR_RUNNING) ||
		(ev3_motor_state( motors->next ) & MOTOR_RUNNING)
	);

	//Let's delete the list in the very end. It will also close the
	//motors
	ev3_delete_motors(motors);

    fclose(fp);

	return 0;
}
