#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"
#include "sensors.h"

//TODO proposition of better way of ordering things to be stored in s and initialized at the beginning
//
typedef struct _mainpositions
{
    position s_fr_init;
    position s_fr_releaseball;
    position s_fr_ending;
} mainpos;

mainpos init_main_positions(){
	mainpos positions;
	position init={.x = S_FR_S_0_X,.y = S_FR_S_0_Y +  WHEELS_TO_END};
	position releaseballposition = {.x = S_BA_0_X,.y = S_BA_0_Y - WHEELS_TO_END};
	position endingposition = {.x = S_FR_E_0_X,.y = (S_FR_E_0_Y + S_FR_E_1_Y) / 2};
	positions.s_fr_init=init;
	positions.s_fr_releaseball=releaseballposition;
	positions.s_fr_ending=endingposition;
	return positions;
}

int first_runner_small_stadium(state *s)
{
    //Init the Game
	position init = {
        .x = S_FR_S_0_X,
        .y = S_FR_S_0_Y +  WHEELS_TO_END		
    };

    update_pos(s, init);

	//catch the ball in front of the robot at the begining
    //catch_ball(s);

    //Go to center
	position releaseballposition = {
        .x = S_BA_0_X,
        .y = S_BA_0_Y - WHEELS_TO_END 
    };

	go_to_pos(s, releaseballposition);

	release(s, MAX_GRABBING_SPEED);

	//Go back a little
	go_straight(s, MAX_WHEEL_SPEED, -15);

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

//Small arena test 1 13 December
int test_one(state *s)
{
    //Init the Game
    mainpos positions=init_main_positions();
    update_pos(s, positions.s_fr_init);

	//Go to ending position
	go_to_pos(s, positions.s_fr_ending);
    return 0;
}

//Small arena test 2 13 December
int test_two(state *s)
{
    //Init the Game
    mainpos positions=init_main_positions();
    update_pos(s, positions.s_fr_init);

    //Go to center
    go_to_pos(s, positions.s_fr_releaseball);
    return 0;
}


