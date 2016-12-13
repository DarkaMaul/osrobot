#include <stdio.h>
#include <unistd.h>

#include "ev3c.h"
#include "tester.h"
#include "main.h"
#include "config.h"
#include "utils.h"
#include "logger.h"

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
                        send_all_messages(s);

            //            printf("NExt! %d\n", send_message(s, MSG_NEXT, s->ally));
                    }
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
