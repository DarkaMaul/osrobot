#include <stdio.h>
#include "ev3c.h"
#include "sensors.h"
#include "main.h"
#include "config.h"
#include "utils.h"
#include "sensors.h"
#include "logger.h"


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
    s->gyro     = ev3_search_sensor_by_port(s->sensors, PORT_SENSOR_GYRO);

    //Configure color sensor
    ev3_open_sensor(s->color);
    ev3_mode_sensor_by_name(s->color, "COL-COLOR");
    log_this(s, "[%s:init_sensors] Color Sensor on port %c opened and configured in mode %d\n", __FILE__, s->color->port,s->color->mode);

    //Configure sonar sensor
    ev3_open_sensor(s->sonar);
    ev3_mode_sensor_by_name(s->sonar, "US-DIST-CM");
    log_this(s, "[%s:init_sensors] Sonar Sensor on port %c opened and configured in mode %d\n", __FILE__, s->sonar->port,s->sonar->mode);

    //Configure Gyro in angle
    ev3_open_sensor(s->gyro);
    ev3_mode_sensor_by_name(s->gyro, "GYRO-ANG");
    log_this(s, "[%s:init_sensors] Gyro Sensor on port %c opened and configured in mode %d\n", __FILE__, s->gyro->port,s->gyro->mode);

    ev3_update_sensor_val(s->gyro);
    s->gyro_reference = s->gyro->val_data[0].s32 + 90; // The 90 is just to get an angle relative to the x axis

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


/**
 * Retrieve the angle from the gyro and clean it
 * @param  s State structure
 * @return int Cleaned angle
 */
int gyro_angle(state *s)
{
    ev3_update_sensor_val(s->gyro);
    int angle = s->gyro->val_data[0].s32;
    return clean_angle(angle - s->gyro_reference);
}


