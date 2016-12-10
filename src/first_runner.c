#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"
#include "sensors.h"
#include "logger.h"

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
	position releaseballposition = {.x = S_BA_0_X - 5,.y = S_BA_0_Y - WHEELS_TO_END + 5};
	position endingposition = {.x = S_FR_E_0_X,.y = ( S_FR_E_0_Y + WHEELS_TO_END) + 15};
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
    log_this(s, "\n\nGrabing the ball.\n\n");
	grab(s, MAX_GRABBING_SPEED);
	
    //Go to center
    log_this(s, "\n\nGoing to the center.\n\n");
	position releaseballposition = {
        .x = S_BA_0_X - 5,
        .y = S_BA_0_Y - WHEELS_TO_END + 5 
    };

	go_to_pos(s, releaseballposition);

    log_this(s, "\n\nReleasing the ball.\n\n");

	release(s, RELEASING_SPEED);


    log_this(s, "\n\nGoing back little.\n\n");
	//Go back a little
	go_straight(s, MAX_WHEEL_SPEED, -20);

    log_this(s, "\n\nGoing to the end.\n\n");
	//Go to ending position
	position endingposition = {
        .x = S_FR_E_0_X ,
        .y = (S_FR_E_0_Y + S_FR_E_1_Y) / 2 + 5
    };

	go_to_pos(s, endingposition);

	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);

    log_this(s, "\n\nTurning around.\n\n");
	turn(s, 70, 180);
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
    turn(s, 70, 180);
    return 0;
}

//Small arena test 2 13 December
int test_two(state *s)
{
    //Init the Game
    mainpos positions=init_main_positions();
    update_pos(s, positions.s_fr_init);
    //By default we are in realeasing position so just close the clamps when starting
    grab(s, MAX_GRABBING_SPEED);
	//time to put the ball inside the hooks
	sleep(3);

    //Go to center
    go_to_pos(s, positions.s_fr_releaseball);

    //release ball
    release(s, MAX_GRABBING_SPEED);
    return 0;
}

//Small arena test 5 13 December
int test_five(state *s)
{
    //Init the Game
    mainpos positions=init_main_positions();
    update_pos(s, positions.s_fr_init);

    //Go to center
//    go_to_pos(s, positions.s_fr_releaseball);
	catch_ball(s);

    return 0;
}


///////////////// LARGE ARENA //////////////////
typedef struct _large_positions 
{
    position l_fr_init;
    position l_fr_dodgefirst;
    position l_fr_releaseball;
    position l_fr_dodgesecond;
    position l_fr_ending;
}large_pos;

large_pos init_large_positions(){
    large_pos large_pos;
    position init = {.x = L_FR_S_0_X + 10, .y = L_FR_S_0_Y + WHEELS_TO_END};
    position dodgefirst = {.x = L_FR_S_0_X + 70, .y = L_FR_S_0_Y + WHEELS_TO_END + 140};
    position center = {.x = L_BA_0_X, .y = L_BA_0_Y - WHEELS_TO_END};
    position dodgesecond = {.x = L_FR_E_0_X + 70, .y = L_FR_E_0_Y + WHEELS_TO_END - 140};
    position ending = {.x = L_FR_E_0_X, .y = L_FR_E_0_Y + WHEELS_TO_END};
    large_pos.l_fr_init = init;
    large_pos.l_fr_dodgefirst = dodgefirst;
    large_pos.l_fr_releaseball = center;
    large_pos.l_fr_dodgesecond = dodgesecond;
    large_pos.l_fr_ending = ending;
    return large_pos;
}

//Large arena test 3 13 December
int test_three(state *s)
{
    //Init the Game
    large_pos positions=init_large_positions();
    update_pos(s, positions.l_fr_init);

	//Go to ending position
    log_this(s, "\n[%s:test3] Dodging first obstacle\n\n" ,__FILE__);
	go_to_pos(s, positions.l_fr_dodgefirst);
    log_this(s, "\n[%s:test3] Dodging second obstacle\n\n" ,__FILE__);
	go_to_pos(s, positions.l_fr_dodgesecond);
    log_this(s, "\n[%s:test3] Going to the end\n\n",__FILE__);
	go_to_pos(s, positions.l_fr_ending);
    return 0;
}

//Large arena test 4 13 December
int test_four(state *s)
{
    //Init the Game
    large_pos positions=init_large_positions();
    update_pos(s, positions.l_fr_init);
    //By default we are in realeasing position so just close the clamps when starting
    log_this(s, "\n[%s:test4] Grabbing the ball \n\n",__FILE__);
    grab(s, MAX_GRABBING_SPEED);
    //Dodge first obstacle
    log_this(s, "\n[%s:test4] Dodging first obstacle \n\n",__FILE__);
	go_to_pos(s, positions.l_fr_dodgefirst);
    //Go to center
    log_this(s, "\n[%s:test3] Going to the center\n\n",__FILE__);
    go_to_pos(s, positions.l_fr_releaseball);

    //release ball
    log_this(s, "\n[%s:test3] Releasing the ball\n\n",__FILE__);
    release(s, MAX_GRABBING_SPEED);
    return 0;
}



