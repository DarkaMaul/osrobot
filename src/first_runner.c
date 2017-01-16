#include <pthread.h>

#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"
#include "sensors.h"
#include "logger.h"

int game_wrapper(state *s, mainpos *p)
{
    //If we have to wait until it's our turn
    if (s->role == ROLE_SECOND)
    {
        //Let's wait for the starting message
        pthread_mutex_lock(&(s->mutexSockUsage));

        char buf[100];
        int returnValue;
        while(1)
        {
            returnValue  = read_message_from_server(s, buf);
            if (returnValue == -1)
            {
                pthread_mutex_unlock(&(s->mutexSockUsage));
                continue;
            }

            if (buf[HEADER_TYPE] == MSG_NEXT)
                break;
            else if (buf[HEADER_TYPE] == MSG_BALL)
                save_ball_position(s, buf);

        }

        pthread_mutex_unlock(&(s->mutexSockUsage));
    }

    //Init the positions



    if (s->type == SMALL_ARENA)
    {
        if(s->role == ROLE_FIRST)
            beginner_small_stadium(s, p);
        else
            finisher_small_stadium(s, p);
    } else
    {
        //TODO
    }

    return 0;
}

int beginner_small_stadium(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->s_fr_init);

    //By default we are in realeasing position so just close the clamps when starting
    log_this(s,"\n\n[%s: beginner_small_stadium] Grabbing the ball\n\n", __FILE__);
    grab(s, MAX_GRABBING_SPEED);

    //Go to center
    log_this(s,"\n\n[%s: beginner_small_stadium] Going to ball area\n\n", __FILE__);
    go_to_pos(s, p->s_fr_ballarea);
    sleep(2);

    //release ball
    log_this(s,"\n\n[%s: beginner_small_stadium] Releasing ball\n\n", __FILE__);
    release(s, RELEASING_SPEED);
    send_message(s, MSG_BALL, s->ally);
    
	//Go back a little
    log_this(s, "\n\n[%s: beginner_small_stadium] Going back a little\n\n", __FILE__);
	go_straight(s, MAX_WHEEL_SPEED, -20);

	//Go to ending position
    log_this(s, "\n\n[%s: beginner_small_stadium] Going to the end\n\n", __FILE__);
    go_to_pos(s, p->s_fr_ending);

	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);

    return 0;

   }

int beginner_large_stadium(state *s, mainpos *p)
{

    //Init the Game
    update_pos(s, p->l_fr_init);

    //By default we are in realeasing position so just close the clamps when starting
    log_this(s, "\n[%s:beginner_large_stadium] Grabbing the ball \n\n",__FILE__);
    grab(s, MAX_GRABBING_SPEED);

    //Dodge first obstacle
    log_this(s, "\n[%s:beginner_large_stadium] Dodging first obstacle \n\n",__FILE__);
	go_to_pos(s, p->l_fr_dodgefirst);

    //Go to center and a 180
    log_this(s, "\n[%s:beginner_large_stadium] Going to the center\n\n",__FILE__);
    go_to_pos(s, p->l_fr_ballarea);
    turn(s, TURNING_SPEED, 180);

    //Release ball
    usleep(100000);
    log_this(s, "\n[%s:beginner_large_stadium] Releasing the ball\n\n",__FILE__);
    release(s, MAX_GRABBING_SPEED);
    send_message(s, MSG_BALL, s->ally);

   	//Go back a little
    log_this(s, "\n[%s: beginner_large_stadium] Going back a little\n\n", __FILE__);
	go_straight(s, MAX_WHEEL_SPEED, -20);

    //Dodge second stadium
    log_this(s, "\n[%s:beginner_large_stadium] Dodging second obstacle\n\n" ,__FILE__);
	go_to_pos(s, p->l_fr_dodgesecond);

    //Go to ending position
    log_this(s, "\n\n[%s: beginner_small_stadium] Going to the end\n\n", __FILE__);
    go_to_pos(s, p->l_fr_ending);


	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);

    return 0;

   }

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
    log_this(s, "\n[%s:beginner_large_stadium] Dodging first obstacle \n\n",__FILE__);
	go_to_pos(s, p->l_fr_dodgefirst);

    //Go to center and a 180
    log_this(s, "\n[%s:beginner_large_stadium] Going to the center\n\n",__FILE__);
    go_to_pos(s, p->l_fr_ballarea);
    turn(s, TURNING_SPEED, 180);

    //Look for the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Looking for the ball\n\n", __FILE__);
    look_for_ball(s);

    //Catch the ball
    log_this(s,"\n\n[%s: finisher_small_stadium] Catching the ball\n\n", __FILE__);
    catch_ball(s);



    //Go to ending position
    log_this(s, "\n\n[%s: beginner_small_stadium] Going to the end\n\n", __FILE__);
    go_to_pos(s, p->l_fr_ending);

	//Send ok signal bluetooth for other team
    send_message(s, MSG_NEXT, s->ally);

    return 0;

}
