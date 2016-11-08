#include <stdio.h>
#include <unistd.h>

#include "ev3c.h"
#include "tester.h"

int testRobot()
{
    ev3_motor_ptr a, b, c;
    ev3_motor_ptr motors = ev3_load_motors();
    ev3_motor_ptr motor = motors;

    FILE* fp = fopen("logs/motortest.txt", "w");

    while (motor)
    {
        ev3_reset_motor(motor);
        ev3_open_motor(motor);

        ev3_command_motor_by_name(motor, "run-forever");

        motor = motor->next;


        fprintf(fp, "Motor openned\n");
    }
/*
    a = ev3_search_motor_by_port(motors, 'A');
    b = ev3_search_motor_by_port(motors, 'B');
    c = ev3_search_motor_by_port(motors, 'C');


    ev3_command_motor_by_name(b, "run-forever");
    ev3_command_motor_by_name(c, "run-forever");

    sleep(5);

    ev3_stop_command_motor_by_name(a, "hold");
    ev3_stop_command_motor_by_name(b, "hold");
    ev3_stop_command_motor_by_name(c, "hold");
*/
    ev3_delete_motors(motors);

    fclose(fp);

    return 0;
}
