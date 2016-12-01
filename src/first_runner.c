#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"



int first_runner_small_stadium(state *s)
{
    //Init the Game
	position init;
    init.x = S_FR_S_0_X;
    init.y = S_FR_S_0_Y +  WHEELS_TO_END;
    update_pos(s, init);

    //TODO Maybe to put it by default at 90
	update_angle(s, 90);

	//catch the ball in front of the robot at the begining
	catch_ball(s);

    //Go to center //TODO by Alexis put the right positions
	position releaseballposition;
	releaseballposition.x = 60;
	releaseballposition.y = 100;
	go_to_pos(s, releaseballposition);
	release(s, MAX_GRABBING_SPEED);

	//Go back a little
	go_straight(s, MAX_WHEEL_SPEED, -30);

	//Go to ending position
	position endingposition;
	endingposition.x=170;
	endingposition.x=170;
	go_to_pos(s, endingposition);

	//Send ok signal bluetooth for other team

    return 0;
}
