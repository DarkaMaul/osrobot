#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "main.h"
#include "logger.h"
#include "ev3c.h"
#include "utils.h"
#include "motors.h"
#include "init.h"
#include "robot.h"
#include "sensors.h"
#include "first_runner.h"
#include "tester.h"
#include "config.h"

state st;
state* s = &st;

mainpos mpos;
mainpos* p = &mpos;

/**
 * Game function : wait until the server send  a start message and process
 * @return void
 */
void game()
{
    //Game function o_OOOOO_o

    char buf[100];
    int returnValue;
    while(1)
    {
        returnValue  = read_message_from_server(s, buf);
        if (returnValue == -1)
            continue;

        if (buf[HEADER_TYPE] == MSG_START)
            break;
    }

    load_game_params(s, buf);
    game_wrapper(s, p);

    return;
}

int main2(int argc, char** argv)
{
    //signal(SIGINT, signal_handler);

    init_robot(s);

    //REMOVE THIS UGLY PART LATER
    s->sock = init_bluetooth();
    if (s->sock == -1)
        nice_exit(s, EXIT_FAILURE);

    game();

    nice_exit(s, EXIT_SUCCESS);

    return 0;
}

int main(int argc, char *argv[])
{
    //Register signal handler
    signal(SIGINT, signal_handler);
    //Reset robot

    if(argc == 2){
        switch(atoi(argv[1])){
            //Test bluetooth
            case 0:
                init_locks(s);
                init_logger(s);
                test_bluetooth(s);
                break;
            //Run beginner
            case 1:
                init_robot(s);
                s->side = -1;
                init_main_positions(s,p);
                beginner_large_stadium(s,p);
                break;
            //Test finisher
            case 2:
                init_robot(s);
                s->side = -1;
                int side = -1;
                position ballpos = {side*L_BA_1_X,L_BA_1_Y} ;
                s->ballPosition = ballpos;
                init_main_positions(s,p);
                finisher_large_stadium(s,p);
                break;
            //Quicks tests
            case 3:
                init_robot(s);
                s->side = 1;
                look_for_ball_mecanical(s);
                //catch_ball(s);
                break;
            case 4:
                main2(0, NULL);
                break;
            case 5:
                init_robot(s);
                position pos = {-20, 10};
                position pos2 = {-10, 80};
                update_pos(s,pos);
                go_to_pos(s, pos2);
                break;
            case 6:
                init_robot(s);
                s->side = 1;
                s->ally = 2;
                s->role = ROLE_FIRST;
                s->type = SMALL_ARENA;
                s->sock =  -1;
                game_wrapper(s, p);
                break;
            case 8:
                init_robot(s);
                s->side = 1;
                s->ally = 2;
                s->role = ROLE_FIRST;
                s->doNotUnitialize = 1;
                s->type = SMALL_ARENA;
                s->sock = -1;

                init_main_positions(s,p);
                s->curPos = p->s_fr_ending;
                s->ballPosition.x = S_BA_2_X;
                s->ballPosition.y = S_BA_2_Y;

                beginner_small_stadium_2(s, p);
                break;

            case 7:
                init_robot(s);
                grab(s, MAX_GRABBING_SPEED);
                getchar();
                release(s, RELEASING_SPEED);
        }
    }
    //Close external ressources
    nice_exit(s, EXIT_SUCCESS);
    return 0;
}
