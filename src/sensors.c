#include <stdio.h>
#include "ev3c.h"
#include "sensors.h"
#include "main.h"
#include "config.h"

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
    //s->gyro     = ev3_search_sensor_by_port(s->sensors, PORT_SENSOR_GYRO);

    //Configure color sensor
    ev3_open_sensor(s->color);
    ev3_mode_sensor_by_name(s->color, "COL-COLOR");

    //Configure sonar sensor
    ev3_open_sensor(s->sonar);
    ev3_mode_sensor_by_name(s->sonar, "US-DIST-CM");

    //ev3_open_sensor(s->gyro);
    //ev3_open_sensor(s->compass);

    //@TODO Configure gyro
    //@TODO  Configure compass

    return 0;
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
