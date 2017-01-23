#include <pthread.h>

#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"
#include "utils.h"
#include "robot.h"
#include "sensors.h"
#include "logger.h"
#include "threads.h"

/**
 * Game wrapper
 * @param  s Structure state
 * @param  p Position
 * @return   Return 0
 */
int game_wrapper(state *s, mainpos *p)
{
    while(1)
    {
        //If we have to wait until it's our turn
        if (s->role == ROLE_SECOND || (s->role == ROLE_FIRST && s->doNotUnitialize ==  1))
        {
            char buf[100];
            int returnValue;
            while(1)
            {
                returnValue  = read_message_from_server(s, buf);
                if (returnValue == -1)
                    continue;

                if (buf[HEADER_TYPE] == MSG_NEXT)
                    break;
                else if (buf[HEADER_TYPE] == MSG_BALL) //Interpret the ball message
                    save_ball_position(s, buf);

            }
        }

        int (*strategy)(state*,mainpos*);
        if (s->type == SMALL_ARENA)
        {
            if(s->role == ROLE_FIRST)
            {
                if (s->side == 1)
                    strategy = &beginner_small_stadium_1;
                else
                    strategy = &beginner_small_stadium_2;

            } else
                strategy = &finisher_small_stadium;

        } else
        {
            if (s->role == ROLE_FIRST)
                strategy = &beginner_large_stadium;
            else
                strategy = &finisher_large_stadium;
        }

        //Start sending position
        pthread_mutex_lock(&(s->mutexGameStarted));
        s->gameStarted = TRAVELLING;
        pthread_mutex_unlock(&(s->mutexGameStarted));

        init_main_positions(s, p);
        strategy(s, p);

        pthread_mutex_lock(&(s->mutexGameStarted));
        s->gameStarted = IMMOBILE;
        pthread_mutex_unlock(&(s->mutexGameStarted));

        send_message(s, MSG_NEXT, s->ally);

        if (s->type == BIG_ARENA)
            break;
    }

    close_threads(s);

    return 0;
}

/**
 * Beginner small stadium 1 strategy (LEADER 1->2)
 * @param  s State structure
 * @param  p Mainpos structure
 * @return   0
 */
int beginner_small_stadium_1(state *s, mainpos *p)
{
    //If it's truely the first run, then do unitialize
    if (s->doNotUnitialize != 1)
        update_pos(s, p->s_fr_init);

    //By default we are in realeasing position so just close the clamps when starting
    //log_this(s,"[%s:beginner_small_stadium_1] Grabbing the ball\n", __FILE__);
    grab(s, MAX_GRABBING_SPEED);

    //Go to center
    log_this(s,"[%s:beginner_small_stadium_1] Going to ball area\n", __FILE__);
    go_to_pos(s, p->s_fr_ballarea);
    getchar();
    position ballcenter = {(S_BA_0_X+S_BA_2_X)/2,(S_BA_0_Y+S_BA_2_Y)/2};
    int rel_angle_to_ball = compute_rel_angle_to_destination(s, ballcenter);
    turn(s,TURNING_SPEED, rel_angle_to_ball);
    update_angle(s, gyro_angle(s));

    //release ball
    log_this(s,"\n[%s:beginner_small_stadium_1] Releasing ball\n", __FILE__);
    release(s, RELEASING_SPEED);
    send_message(s, MSG_BALL, s->ally);

    //Go back a little
    log_this(s, "\n[%s:beginner_small_stadium_1] Going back a little\n", __FILE__);
    go_straight(s, MAX_WHEEL_SPEED, -20);
    getchar();

    //Go to ending position
    log_this(s, "\n[%s:beginner_small_stadium_1] Going to the end\n", __FILE__);
    go_to_pos(s, p->s_fr_ending);

    return 0;
}

/**
 * Beginner small stadium : stragegy LEADER (2->1)
 * @param  s State structure
 * @param  p Mainpos structure
 * @return  0
 */
int beginner_small_stadium_2(state *s, mainpos *p)
{
    //We already know where we are
    //We already have open clamps

    //Go to ball
    log_this(s,"\n[%s:beginner_small_stadium_2] Going to ball area\n", __FILE__);
    //position ballArea = s->ballPosition;
    //ballArea.y +=  20;
    go_to_pos(s, compute_arrival_point(s));

    //Look for it and catch it!
    log_this(s,"\n[%s:beginner_small_stadium_2] Trying to catch the ball\n", __FILE__);
    getchar();
    if (look_for_ball(s) == 0)
        catch_ball(s);

    //Go to ending position
    log_this(s, "\n[%s:beginner_small_stadium_2] Going to the end\n", __FILE__);
    go_to_pos(s, p->s_fr_init);

    //Turn to be ready for next time
    turn(s, HIGH_TURNING_SPEED, 180);
    release(s, RELEASING_SPEED);

    //Prevent beginner_small_stadium_1 to unitialize the position
    s->doNotUnitialize = 1;

    return 0;
}

/**
 * Beginner large stadium : Strategy LEADER
 * @param  s State structure
 * @param  p Mainpos structure
 * @return 0
 */
int beginner_large_stadium(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->l_fr_init);

    //By default we are in realeasing position so just close the clamps when starting
    log_this(s, "\n[%s:beginner_large_stadium] Grabbing the ball \n",__FILE__);
    grab(s, MAX_GRABBING_SPEED);

    //Dodge first obstacle
    log_this(s, "\n[%s:beginner_large_stadium] Dodging first obstacle \n",__FILE__);
    go_to_pos(s, p->l_fr_dodgefirst);
    getchar();

    //Go to center and do a 180
    log_this(s, "\n[%s:beginner_large_stadium] Going to the center\n",__FILE__);
    go_to_pos(s, p->l_fr_ballarea);
    getchar();
    turn(s, HIGH_TURNING_SPEED, 180);
    update_angle(s, gyro_angle(s));
    getchar();

    //Release ball
    log_this(s, "\n[%s:beginner_large_stadium] Releasing the ball\n",__FILE__);
    release(s, RELEASING_SPEED);
    send_message(s, MSG_BALL, s->ally);

    //Go back a little
    log_this(s, "\n[%s:beginner_large_stadium] Going back a little\n", __FILE__);
    go_straight(s, MAX_WHEEL_SPEED, -20);

    //Go to ending position
    log_this(s, "\n[%s: beginner_large_stadium] Going to the end\n", __FILE__);
    go_to_pos(s, p->l_fr_ending);

    return 0;

   }

/**
 * Finisher Small stadium : strategy FOLLOWER
 * @param  s State structure
 * @param  p Mainpos structure
 * @return 0
 */
int finisher_small_stadium(state *s, mainpos *p)
{
    //Init the Game
    if (s->doNotUnitialize != 1)
    {
        update_pos(s, p->s_sr_init);
        update_angle(s,90);
        s->gyro_reference -= 180; //To set the starting gyro value to 90° (clockwise)
    }

    //Go to center
    log_this(s,"\n[%s:finisher_small_stadium_1] Going to ball area\n", __FILE__);
    go_to_pos(s, compute_arrival_point(s));

    //Look for the ball
    log_this(s,"\n[%s:finisher_small_stadium_1] Looking for the ball\n", __FILE__);
    if (look_for_ball(s) == 0)
        catch_ball(s);

      //Go to ending position
    log_this(s, "\n[%s:finisher_small_stadium_1] Going to the end\n", __FILE__);
    go_to_pos(s, p->s_sr_ending);

    // ------------------- SECOND TRIP ---------------
    //Turn the robot
    turn(s, HIGH_TURNING_SPEED, 180);

    //Go to center
    log_this(s,"\n[%s:finisher_small_stadium_2] Going to ball area\n", __FILE__);
    go_to_pos(s, p->s_sr_ballarea);

    //Look for the ball
    log_this(s,"\n[%s:finisher_small_stadium_2] Release the ball\n", __FILE__);
    release(s, RELEASING_SPEED);
    send_message(s, MSG_BALL, s->ally);

    //Go back a little
    go_straight(s, MAX_WHEEL_SPEED, -20);

    //Go to ending position
    log_this(s, "\n[%s:finisher_small_stadium_2] Going to the end\n", __FILE__);
    go_to_pos(s, p->s_sr_init);

    turn(s, HIGH_TURNING_SPEED, 180);

    //We don't want intialization next time
    s->doNotUnitialize = 1;

    return 0;
}

/**
 * Finisher large stadium: strategy FOLLOWER
 * @param  s State structure
 * @param  p Mainpos structure
 * @return 0
 */
int finisher_large_stadium(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->l_sr_init);
    update_angle(s, 90);
    s->gyro_reference -= 180;

    //Dodge first obstacle
    log_this(s, "\n[%s:finisher_large_stadium] Dodging first obstacle \n",__FILE__);
	go_to_pos(s, p->l_sr_dodgefirst);

    //Go to center and a 180
    log_this(s, "\n[%s:finisher_large_stadium] Going to the center\n",__FILE__);
    //go_to_pos(s, p->l_sr_ballarea);
    //go_to_pos(s, compute_arrival_point(s));
    int rel_angle_to_ball = compute_rel_angle_to_destination(s,s->ballPosition);
    //printf("\nTurning towards the ball %d \n\n",rel_angle_to_ball);
    //int rel_angle_to_ball = -(s->side*45);
    turn(s,TURNING_SPEED, rel_angle_to_ball);
    update_angle(s, gyro_angle(s));

    //Look for the ball
    log_this(s,"\n[%s:finisher_large_stadium] Looking for the ball\n", __FILE__);
    if (look_for_ball(s) == 0)
        catch_ball(s);

    //Dodge second obstacle
    log_this(s, "\n[%s:finisher_large_stadium] Dodging first obstacle \n",__FILE__);
	go_to_pos(s, p->l_sr_dodgesecond);

    //Go to ending position
    log_this(s, "\n[%s: finisher_large_stadium] Going to the end\n", __FILE__);
    go_to_pos(s, p->l_sr_ending);

    return 0;
}
