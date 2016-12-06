#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"
#include "sensors.h"

int first_runner_small_stadium(state *s)
{
    //Init the Game
	position init = {
        .x = S_FR_S_0_X,
        .y = S_FR_S_0_Y +  WHEELS_TO_END		
    };

    update_pos(s, init);

    //TODO Maybe to put it by default at 90
	update_angle(s, 90);

	//catch the ball in front of the robot at the begining
    //catch_ball(s);

    //Go to center
	position releaseballposition = {
        .x = S_BA_0_X,
        .y = S_BA_0_Y 
    };

	go_to_pos(s, releaseballposition);

	release(s, MAX_GRABBING_SPEED);

	//Go back a little
	go_straight(s, MAX_WHEEL_SPEED, -30);

	//Go to ending position
	position endingposition = {
        .x = S_FR_E_0_X,
        .y = (S_FR_E_0_Y + S_FR_E_1_Y) / 2
    };

	go_to_pos(s, endingposition);

	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);
    return 0;
}

int second_runner_small_stadium(state *s)
{
    position initPosition = {
        .x = S_SR_E_2_X,
        .y = S_SR_E_2_Y + (S_SR_E_2_Y - S_SR_E_3_Y) / 2
    };
    
    update_pos(s, initPosition);
    
    position checkBallPosition = {
        .x = S_BA_1_X,
        .y = S_BA_1_Y
    };
   
    go_to_pos(s, checkBallPosition);
    
    int distanceToBall = distance_from_obstacle(s);
    int sweep = 0;
    while(distanceToBall == -1)
    {
        //Positive for clockwise turn
        if (abs(sweep * SWEEP_ANGLE) < MAX_SWEEP_ANGLE) 
            turn(s, TURNING_SPEED, -SWEEP_ANGLE);
        else
            turn(s, TURNING_SPEED, SWEEP_ANGLE);
        
        sleep(1);
        distanceToBall = distance_from_obstacle(s);
        sweep++;
    }

    return 0;
}
