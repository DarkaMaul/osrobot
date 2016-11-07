#ifndef MAIN_H
#define MAIN_H
#include "ev3c.h"

typedef struct _state
{
	/* motors and sensors */
        ev3_motor_ptr motors;
        ev3_sensor_ptr sensors;

        ev3_motor_ptr m_dx, m_sx, gripper;
        ev3_sensor_ptr us, gy, co;

	/* flags */
        int gripper_state;
        int find_state, grab_state;
	int action_decided;
	int send_new_pkt;
        int moving, sleeping;

	/* wall-ev3's position and movements*/
        int x;
        int y;
        int angle;

        int next_mov_x, next_mov_y;
        int next_abs_mov_x, next_abs_mov_y;
        int next_d;
        int next_angle;

	/* leader's position and movements */
        int x_leader;
        int y_leader;
        int angle_leader;
	int speed_leader;
	int leader_last_d;

	/* bluetooth */
        int last_acked, action_acked;
        int sock;

	/* snake status */
        uint16_t id;
        uint8_t rank, size, follower_id, prev_id;

	/* log file fd */
        FILE * logfile_fd;
} state;

#endif
