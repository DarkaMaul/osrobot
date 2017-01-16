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



//Small arena test 1 13 December
int test_one(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->s_fr_init);

	//Go to ending position
	go_to_pos(s, p->s_fr_ending);
    turn(s, 70, 180);
    return 0;
}

//Small arena test 2 13 December
int test_two(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->s_fr_init);
    //By default we are in realeasing position so just close the clamps when starting
    grab(s, MAX_GRABBING_SPEED);
	//time to put the ball inside the hooks
	sleep(3);

    //Go to center
    go_to_pos(s, p->s_fr_ballarea);
    sleep(2);
    //release ball
    release(s, RELEASING_SPEED);
    return 0;
}

//Small arena test 5 13 December
int test_five(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->s_fr_init);

    //Go to center
//    go_to_pos(s, p->s_fr_ballarea);
	catch_ball(s);

    return 0;
}


///////////////// LARGE ARENA //////////////////

//Large arena test 3 13 December
int test_three(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->l_fr_init);

	//Go to ending position
    log_this(s, "\n[%s:test3] Dodging first obstacle\n\n" ,__FILE__);
	go_to_pos(s, p->l_fr_dodgefirst);
    log_this(s, "\n[%s:test3] Dodging second obstacle\n\n" ,__FILE__);
	go_to_pos(s, p->l_fr_dodgesecond);
    log_this(s, "\n[%s:test3] Going to the end\n\n",__FILE__);
	go_to_pos(s, p->l_fr_ending);
    turn(s,100,180);
    return 0;
}

//Large arena test 4 13 December
int test_four(state *s, mainpos *p)
{
    //Init the Game
    update_pos(s, p->l_fr_init);
    //By default we are in realeasing position so just close the clamps when starting
    log_this(s, "\n[%s:test4] Grabbing the ball \n\n",__FILE__);
    grab(s, MAX_GRABBING_SPEED);
    //Dodge first obstacle
    log_this(s, "\n[%s:test4] Dodging first obstacle \n\n",__FILE__);
	go_to_pos(s, p->l_fr_dodgefirst);
    //Go to center
    log_this(s, "\n[%s:test3] Going to the center\n\n",__FILE__);
    go_to_pos(s, p->l_fr_ballarea);

    //release ball
    usleep(100000);
    log_this(s, "\n[%s:test3] Releasing the ball\n\n",__FILE__);
    release(s, MAX_GRABBING_SPEED);
    return 0;
}

int test_six(state *s, mainpos *p)
{
    printf("Test 6\n");
    int distanceToBall = distance_from_obstacle(s);
    printf("Distance to ball (0): %d\n", distanceToBall);

    int i;
    for (i = 0; i < 13; i++)
    {
        if (i == 5)
        {
            printf("Going forward and turning back\n");
            turn(s, TURNING_SPEED, i * -5);
            go_straight(s, MAX_WHEEL_SPEED, 30);
        }

        turn(s, TURNING_SPEED, 5);
        usleep(500000);
        distanceToBall = distance_from_obstacle(s);
        printf("Distance to ball (%d): %d\n", i, distanceToBall);
        if (distanceToBall != -1 && distanceToBall < 40)
        {
            turn(s,TURNING_SPEED,8);
            break;
        }
    }

    catch_ball(s);

    return 0;
}
