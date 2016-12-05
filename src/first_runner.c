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
    //catch_ball(s);

    //Go to center
	position releaseballposition;
	releaseballposition.x = S_BA_0_X + (S_BA_3_X -  S_BA_0_X) / 2;
	releaseballposition.y = S_BA_3_Y + (S_BA_2_Y - S_BA_3_Y) /2 ;

	go_to_pos(s, releaseballposition);

	release(s, MAX_GRABBING_SPEED);

	//Go back a little
	go_straight(s, MAX_WHEEL_SPEED, -30);

	//Go to ending position
	position endingposition;
	endingposition.x=S_FR_E_0_X;
	endingposition.y= (S_FR_E_0_Y + S_FR_E_1_Y) / 2;

	go_to_pos(s, endingposition);

	//Send ok signal bluetooth for other team

    return 0;
}

int second_runner()
{
    return 0;
}
