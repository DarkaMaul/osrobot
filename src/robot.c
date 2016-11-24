#include <stdio.h>
#include "ev3c.h"
#include "sensors.h"
#include "main.h"
#include "config.h"
#include "motors.h"
#include "logger.h"
/**

 * Catch the ball if it's in front of the robot
 * @param  s State structure
 * @return   BALL_FOUND || BALL_NOT_FOUND
 */
int catchBall(state* s)
{
    log_this(s, "[%s] Trying to catch the ball.\n", __FILE__);

    //First release the hook
    release(s, MAX_GRABBING_SPEED);

    //Then check if the ball is  really there
    int distanceBall = distance_from_obstacle(s);

    //The ball was not found in a confident interval
    if (distanceBall == -1)
    {
        log_this(s, "[%s] No ball found in front of the robot... :(  \n", __FILE__);
        return -1;
    }

    //Run to the ball
    go_straight(s, MAX_WHEEL_SPEED, distanceBall + BALL_CATCH_DISTANCE);

    //Check if the ball is inside the hook
    if (is_ball_present(s) == BALL_NOT_FOUND)
    {
        log_this(s, "[%s] Sadly, the ball is not inside  the hook...\n", __FILE__);
        return BALL_NOT_FOUND;
    }

    //Close the hook
    grab(s, MAX_GRABBING_SPEED);

    int returnValue = is_ball_present(s);
    if(returnValue == BALL_FOUND)
        log_this(s, "[%s] We're lucky, the ball is still inside the hook!\n", __FILE__);
    else
        log_this(s, "[%s] Damm, we missed it at the very last moment\n", __FILE__);

    return returnValue;
}
