#include <stdio.h>
#include <unistd.h>

#include "ev3c.h"
#include "tester.h"
#include "main.h"
#include "config.h"
#include "utils.h"

void test_bluetooth(state *s)
{
    s->sock = init_inet(s);

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
                        printf("Ally: %u\n", s->ally);
                        printf("Send NEXT...");
                        result = send_message(s, MSG_NEXT, s->ally);
                        printf(".... %d\n", result);
                    }
                    break;

                case MSG_NEXT:
                    printf("MESSAGE NEXT RECIEVIED\n");
                    s->curPos.x = -100;
                    s->curPos.y = 243;
                    result = send_position(s);
                    result = send_message(s, MSG_BALL, s->ally, PICK_BALL, -252, 1224);
                    printf("Send ball: %d\n", result);
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
