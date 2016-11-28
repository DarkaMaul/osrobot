#include <stdio.h>
#include "ev3c.h"
#include "sensors.h"
#include "main.h"
#include "config.h"
#include "utils.h"

/**
 * Init the sensors
 * @param  s State structure
 * @return   0 in case of success, 1 in case of error
 */
int init_sensors(state *s)
{
    s->sensors = ev3_load_sensors();
    s->color    = ev3_search_sensor_by_port(s->sensors, PORT_SENSOR_COLOR);
    s->sonar    = ev3_search_sensor_by_port(s->sensors, PORT_SENSOR_SONAR);
    //s->compass  = ev3_search_sensor_by_port(s->sensors, PORT_SENSOR_COMPASS);
    s->gyro     = ev3_search_sensor_by_port(s->sensors, PORT_SENSOR_GYRO);

    //Configure color sensor
    ev3_open_sensor(s->color);
    ev3_mode_sensor_by_name(s->color, "COL-COLOR");

    //Configure sonar sensor
    ev3_open_sensor(s->sonar);
    ev3_mode_sensor_by_name(s->sonar, "US-DIST-CM");

    ev3_open_sensor(s->gyro);
    //ev3_open_sensor(s->compass);

    //@TODO Configure gyro
    //Configure Gyro in angle
    ev3_mode_sensor_by_name(s->gyro, "GYRO-ANG");
    //@TODO  Configure compass

    s->gyro_reference = s->gyro->val_data[0].s32;

    return 0;
}

/**
 * Check if the ball is in front of the captor
 *
 * @param  s State structure
 * @return   0 if found || -1 otherwise
 */
int is_ball_present(state *s)
{
    ev3_update_sensor_val(s->color);
    int valColor = s->color->val_data[0].s32;

    if (valColor == BALL_COLOR_1 || valColor == BALL_COLOR_2)
        return BALL_FOUND;

    return BALL_NOT_FOUND;
}

/**
 * Return the distance in cm from the front obstacle
 * @param  s Structure State
 * @return   -1 if the value is not trustable || the distance
 */
int distance_from_obstacle(state *s)
{
    ev3_update_sensor_val(s->sonar);
    int valUS = s->sonar->val_data[0].s32;

    if (valUS == US_MAX_RETURN_VAL)
        return -1;
    else if (valUS > US_MAX_DISTANCE || valUS < US_MIN_DISTANCE)
        return -1;
    else
        return valUS / 10;
}

/*
* Returns the angle of the robot, between 0 and 360, 0 being the initial angle of the robot
*/  
int gyro_angle(state *s)
{
    ev3_update_sensor_val(s->gyro);
    int angle = s->gyro->val_data[0].s32;
    printf("Angle:%d\t%d\n", angle, clean_angle(angle - s->gyro_reference));
    return clean_angle(angle - s->gyro_reference);
}


int testSensor()
{
	int i;
	//Loading all sensors
	ev3_sensor_ptr sensors = ev3_load_sensors();
	//I myself have a nxt sound sensor, which I detect here and set the
	//correct driver. If you don't have an nxt analog sensor connected
	//nothing should happen. If you have another than the sound sensor
	//connected the values will be interpreted as sound values. Adjust
	//this line in that case.
	ev3_driver_sensor(
		ev3_search_sensor_by_identifier( sensors, NXT_ANALOG, 0 ),
		"lego-nxt-sound"
	);
	//Interating over the sensors and printing some interesting data
	ev3_sensor_ptr sensor = sensors;
	while (sensor)
	{
		printf("==== %s ====\n",sensor->driver_name);
		printf("ident: %i\n",sensor->driver_identifier);
		printf("sensor: %i\n",sensor->sensor_nr);
		printf("port: %i\n",sensor->port);
		printf("bin_fd: %i\n",sensor->bin_fd);
		printf("data count: %i\n",sensor->data_count);
		printf("data format: %i\n",sensor->bin_data_format);
		printf("units: %s\n",sensor->units);
		printf("decimals: %i\n",sensor->decimals);
		printf("poll ms: %i\n",sensor->poll_ms);
		printf("mode count: %i\n",sensor->mode_count);
		for (i = 0; i < sensor->mode_count; i++)
			printf("mode[%i]: %s\n",i,sensor->modes[i]);
		printf("active mode: %i\n",sensor->mode);
		//If there is more than one mode, let's choose the second one.
		if (sensor->mode_count > 1)
			ev3_mode_sensor(sensor,1);

		//Let's open the sensors ;)
		ev3_open_sensor(sensor);
		sensor = sensor->next;
	}
	//Let's do this again, but this time we print the bin and val data
	//values for every found sensor for the next 10 seconds
	printf("Showing the sensor values for the next 10 seconds\n");
	for (i = 0; i < 10; i++)
	{
		sensor = sensors;
		while (sensor)
		{
			ev3_update_sensor_bin(sensor);
			ev3_update_sensor_val(sensor);
			printf("%s [%i]: \n",sensor->driver_name,sensor->port);
			int j;
			for (j = 0; j < sensor->data_count;j++)
				printf("\tvalue %i: %i (raw) - %i (formated)\n",j,sensor->bin_data[0].s32,sensor->val_data[0].s32);
			sensor = sensor->next;
		}
		sleep(1);
	}
	//Let's delete the list in the very end. It will also close the
	//sensors
	ev3_delete_sensors(sensors);
	return 0;
}
