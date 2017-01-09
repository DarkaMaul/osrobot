#include <signal.h>

#include "init.h"
#include "main.h"
#include "sensors.h"
#include "logger.h"
#include "motors.h"
#include "utils.h"

extern state* s;

void signal_handler(int signalNumber)
{
    if (signalNumber ==  SIGINT)
    {
        log_this(s, "[Init] Recvieved SIGINT.\n");
        nice_exit(s, EXIT_FAILURE);
    }
}

/**
 * Initialize LeE
 * @param  s Structure state
 * @return   0 in case of success || 1 in case of error
 */
int init_robot(state *s, mainpos *p )
{
    int returnValue;
    //Initialize the logger
    init_logger(s);

    //Initialize sockets
    /*s->sock = init_inet(s);
    if (s->sock == -1)
        nice_exit(s, EXIT_FAILURE);
    */

    //Init the motors
    init_motors(s);

    //Init sensors
    returnValue = init_sensors(s);
    if (returnValue !=  0)
    {
        log_this(s, "[%s] Sensors unable to initialize.\n", __FILE__);
        return 1;
    }

    //Init position
    init_pos(s);

    //Init main positions for the game
    init_main_positions(p);
    return 0;
}

/**
* Initialize the positions used by LeE
*@param positions The positions variable defined in main
**/
void init_main_positions(mainpos *p){
   
    position s_fr_init={.x = S_FR_S_0_X,.y = S_FR_S_0_Y +  WHEELS_TO_END};
    position s_fr_ballareaposition = {.x = S_BA_0_X - 5,.y = S_BA_0_Y - WHEELS_TO_END + 5};
    position s_fr_endingposition = {.x = S_FR_E_1_X,.y = S_FR_E_1_Y};
    position l_fr_init = {.x = L_FR_S_0_X + WHEELS_TO_END, .y = L_FR_S_0_Y + WHEELS_TO_END};
    position l_fr_dodgefirst = {.x = (L_O1_2_X + BIG_ARENA_MAX_X)/2, .y = L_O1_2_Y + L_FR_S_0_Y};
    position l_fr_center = {.x = L_BA_0_X + L_FR_S_0_X/2, .y = L_BA_0_Y + L_FR_S_0_Y};
    position l_fr_dodgesecond = {.x = L_FR_E_0_X , .y = L_O2_0_Y};
    position l_fr_ending = {.x = L_FR_E_1_X, .y = L_FR_E_1_Y};


    position s_sr_init={.x = S_SR_S_0_X + ROBOT_WIDTH,.y = S_SR_S_0_Y +  WHEELS_TO_END};
    position s_sr_ballareaposition = {.x = S_BA_1_X - 5,.y = S_BA_1_Y + WHEELS_TO_END + 5};
    position s_sr_endingposition = {.x = S_SR_E_1_X + ROBOT_WIDTH, .y = S_SR_E_1_Y - WHEELS_TO_END};
    position l_sr_init = {.x = L_FR_S_0_X + WHEELS_TO_END, .y = L_FR_S_0_Y + WHEELS_TO_END};
    position l_sr_dodgefirst = {.x = (L_O1_2_X + BIG_ARENA_MAX_X)/2, .y = L_O1_2_Y + L_FR_S_0_Y};
    position l_sr_center = {.x = L_BA_0_X + L_FR_S_0_X/2, .y = L_BA_0_Y + L_FR_S_0_Y};
    position l_sr_dodgesecond = {.x = L_FR_E_0_X , .y = L_O2_0_Y};
    position l_sr_ending = {.x = L_FR_E_1_X, .y = L_FR_E_1_Y};

    p->s_fr_init=s_fr_init;
	p->s_fr_ballarea=s_fr_ballareaposition;
	p->s_fr_ending=s_fr_endingposition;
	p->l_fr_init = l_fr_init;
	p->l_fr_dodgefirst = l_fr_dodgefirst;
	p->l_fr_ballarea = l_fr_center;
	p->l_fr_dodgesecond = l_fr_dodgesecond;
	p->l_fr_ending = l_fr_ending;


    p->s_sr_init=s_sr_init;
    p->s_sr_ballarea=s_sr_ballareaposition;
    p->s_sr_ending=s_sr_endingposition;
    p->l_sr_init = l_sr_init;
    p->l_sr_dodgefirst = l_sr_dodgefirst;
    p->l_sr_ballarea = l_sr_center;
    p->l_sr_dodgesecond = l_sr_dodgesecond;
    p->l_sr_ending = l_sr_ending;
}


/**
 * Nice exit the robot
 * @param s         Structure state
 * @param exitState EXIT_SUCCESS || EXIT_FAILURE
 */
void nice_exit(state *s, int exitState)
{
    log_this(s, "[Main] Nice exit called.\n");

    //TODO @CLOSE BLUETOOTH
    if (s->sock > 0)
        close_inet(s);

    if (s->logfile_fd > 0)
        close_logger(s);

    ev3_delete_motors(s->motors);
    exit(exitState);
}
