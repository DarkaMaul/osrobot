#include "main.h"
#include "config.h"
#include "first_runner.h"
#include "motors.h"

int first_runner_small_stadium(state *s)
{
    //Init the Game
    s->curPos.x = S_FR_S_0_X;
    s->curPos.y = S_FR_S_0_Y +  WHEELS_TO_END;


    return 0;
}
