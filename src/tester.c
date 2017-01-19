#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "ev3c.h"
#include "tester.h"
#include "main.h"
#include "config.h"
#include "utils.h"
#include "logger.h"
#include "first_runner.h"
#include "motors.h"
#include "robot.h"
#include "sensors.h"
#include "logger.h"
#include "threads.h"
#include "init.h"

void send_all_messages(state *s)
{
    log_this(s, "[%s] Send all messages\n", __FILE__);

    int result = 0;

    s->curPos.x = -100;
    s->curPos.y = 243;
    //result = send_position(s);
    printf("Position: %d\n", result);

    printf("Ally: %d\n", (unsigned int) s->ally);

    result = send_message(s, MSG_BALL, s->ally, PICK_BALL, -252, 1224);
    printf("Ball: %d\n", result);

    result = send_message(s, MSG_NEXT, s->ally);
    printf("Next: %d\n", result);
}

void test_bluetooth(state *s)
{
    s->sock = init_inet(s);
    init_locks(s);

    //Set game started
    pthread_mutex_lock(&(s->mutexGameStarted));
    s->gameStarted = IMMOBILE;
    pthread_mutex_unlock(&(s->mutexGameStarted));

    //Init Threads
    if(pthread_create(&(s->threadPosition), NULL, (void *) position_thread, (void*) &s))
    {
        printf("[%s] Unable to create position thread\n", __FILE__);
        exit(0);
    }

    game();
    exit(0);

    char buffer[MSG_MAX_LEN];
    int readedBytes, result;
    while (1)
    {
        sleep(1);
        readedBytes = read_message_from_server(s, buffer);
        if (readedBytes != -1)
        {
            switch (buffer[HEADER_TYPE])
            {
                case MSG_START:
                    result = load_game_params(s, buffer);
                    if (result == 0)
                    {
                        printf("Message START recieved and well interpreted\n");
                        mainpos *p;
                        game();

                        //send_all_messages(s);

            //            printf("NExt! %d\n", send_message(s, MSG_NEXT, s->ally));
                    }
                    break;

                case MSG_BALL:
                    printf("MESSAGE BALL RECIEVIED\n");
                    save_ball_position(s, buffer);
                    break;

                case MSG_NEXT:
                    printf("MESSAGE NEXT RECIEVIED\n");
                    break;

                case MSG_STOP:
                    printf("MESSAGE STOP recieved\n");
                    return;
                    break;

                case MSG_KICK:
                    if (buffer[5] == TEAM_ID)
                    {
                        printf("We have been kicked ...\n");
                        return;
                    }
                    else
                        printf("Someone else has been kicked !\n");

                    break;

                default:
                    break;

            }
        }
    }

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
