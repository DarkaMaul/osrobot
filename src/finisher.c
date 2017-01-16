#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"
#include "sensors.h"
#include "logger.h"



int finisher_small_stadium(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->s_sr_init);
    update_angle(s,90);
    s->gyro_reference -= 180; //To set the starting gyro value to 90° (clockwise) 

    //TODO replace center by ball position sent by the beginner
    //Go to center
    log_this(s,"\n\n[%s: finisher_small_stadium] Going to ball area\n\n", __FILE__);
    go_to_pos(s, p->s_sr_ballarea);

    //Look for the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Looking for the ball\n\n", __FILE__);
    look_for_ball(s);

    //Catch the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Catching the ball\n\n", __FILE__);
    catch_ball(s);

  	//Go to ending position
    log_this(s, "\n\n[%s: beginner_small_stadium] Going to the end\n\n", __FILE__);
    go_to_pos(s, p->s_sr_ending);

	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);

    return 0;

}

int finisher_large_stadium(state *s, mainpos *p)
{   

    //Init the Game
    update_pos(s, p->l_fr_init);
   
    //Dodge first obstacle
    log_this(s, "\n[%s:finisher_large_stadium] Dodging first obstacle \n\n",__FILE__);
	go_to_pos(s, p->l_fr_dodgefirst);
    
    //Go to center and a 180
    log_this(s, "\n[%s:finisher_large_stadium] Going to the center\n\n",__FILE__);
    go_to_pos(s, p->l_fr_ballarea);
    turn(s, TURNING_SPEED, 180);
    
    //Look for the ball
    log_this(s,"\n\n[%s: finisher_large_stadium] Looking for the ball\n\n", __FILE__);
    look_for_ball(s);

    //Catch the ball
    log_this(s,"\n\n[%s: finisher_large_stadium] Catching the ball\n\n", __FILE__);
    catch_ball(s);

    //Go to ending position
    log_this(s, "\n\n[%s: finisher_large_stadium] Going to the end\n\n", __FILE__);
    go_to_pos(s, p->l_fr_ending);

	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);

    return 0;

}
