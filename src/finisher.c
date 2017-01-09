#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"
#include "sensors.h"
#include "logger.h"
/*
//TODO adapt the struct to the finisher (I only changed the names)
//
typedef struct _mainpositions
{
    position s_sr_init;
    position s_sr_releaseball;
    position s_sr_ending;
    position l_sr_init;
    position l_sr_dodgefirst;
    position l_sr_releaseball;
    position l_sr_dodgesecond;
    position l_sr_ending;
} mainpos;

mainpos init_main_positions(){
	mainpos positions;
	position s_sr_init={.x = S_SR_S_0_X,.y = S_SR_S_0_Y +  WHEELS_TO_END};
	position s_sr_releaseballposition = {.x = S_BA_0_X - 5,.y = S_BA_0_Y - WHEELS_TO_END + 5};
	position s_sr_endingposition = {.x = S_SR_E_1_X,.y = S_SR_E_1_Y};
	position l_sr_init = {.x = L_FR_S_0_X + WHEELS_TO_END, .y = L_FR_S_0_Y + WHEELS_TO_END};
	position l_sr_dodgefirst = {.x = (L_O1_2_X + BIG_ARENA_MAX_X)/2, .y = L_O1_2_Y + L_FR_S_0_Y};
	position l_sr_center = {.x = L_BA_0_X + L_FR_S_0_X/2, .y = L_BA_0_Y + L_FR_S_0_Y};
	position l_sr_dodgesecond = {.x = L_FR_E_0_X , .y = L_O2_0_Y};
	position l_sr_ending = {.x = L_FR_E_1_X, .y = L_FR_E_1_Y};

	positions.s_sr_init=s_sr_init;
	positions.s_sr_releaseball=s_sr_releaseballposition;
	positions.s_sr_ending=s_sr_endingposition;
	positions.l_sr_init = l_sr_init;
	positions.l_sr_dodgefirst = l_sr_dodgefirst;
	positions.l_sr_releaseball = l_sr_center;
	positions.l_sr_dodgesecond = l_sr_dodgesecond;
	positions.l_sr_ending = l_sr_ending;
	return positions;
}



int finisher_small_stadium(state *s)
{
    //Init the Game
    mainpos positions=init_main_positions();
    update_pos(s, positions.s_sr_init);

    //TODO replace center by ball position sent by the beginner
    //Go to center
    log_this(s,"\n\n[%s: finisher_small_stadium] Going to ball area\n\n", __FILE__);
    go_to_pos(s, positions.s_sr_releaseball);

    //Look for the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Looking for the ball\n\n", __FILE__);
    look_for_ball(s);

    //Catch the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Catching the ball\n\n", __FILE__);
    catch_ball(s);

  	//Go to ending position
    log_this(s, "\n\n[%s: beginner_small_stadium] Going to the end\n\n", __FILE__);
    go_to_pos(s, positions.s_sr_ending);

	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);

    return 0;

}

*/
