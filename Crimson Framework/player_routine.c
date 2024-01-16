#include "framework.h"

// Helper functions
void player_air_restore_angle(child_player* ent)
{
	if (ent->angle != 0) {
		if (ent->angle < 0) {
			ent->angle += 2;
			if (ent->angle >= 0) {
				ent->angle = 0;
			}
		}
		else if (ent->angle > 0) {
			ent->angle -= 2;
			if (ent->angle <= 0) {
				ent->angle = 0;
			}
		}
	}
}

// Routine branching
void player_routine_update(child_player* ent)
{
	switch (ent->routine)
	{
	case PROUTINE_NONE:
		player_routine_none(ent);
		break;

	case PROUTINE_CONTROL:
		player_routine_control(ent);
		break;
	}
}

// ROUTINES

void player_routine_none(child_player* ent)
{
	printf("player %d is in the 'NONE' routine!\n", ent->inst->id);
}

void player_routine_control(child_player* ent)
{
	switch (ent->mode)
	{
	case PMODE_NONE:
		printf("player %d is in the 'NONE' mode!\n", ent->inst->id);
		break;

	case PMODE_GROUND:

		//printf("ground\n");

		// Reset jumping status
		//ent->jumping = false;
		ent->jump_flag = 0;
		
		// Run check to see if we should jump. If no jump happened, continue with the normal ground code.
		if (!player_jump_do(ent)) {
			if (ent->grounded) {
				if ((!ent->input->RIGHT_HOLD && !ent->input->LEFT_HOLD) || (ent->input->RIGHT_HOLD && ent->input->LEFT_HOLD)) {

					if (ent->gspeed > 0) { ent->gspeed -= ent->ground_friction; }
					if (ent->gspeed < 0) { ent->gspeed += ent->ground_friction; }

					if (CR_ABS(ent->gspeed) <= ent->ground_friction || CR_ABS(ent->gspeed) <= ent->ground_accel) { ent->gspeed = 0; }
				}
				else {
					if (ent->input->RIGHT_HOLD && !ent->input->LEFT_HOLD) { // If holding right but not left
						if (ent->gspeed > 0) { // If moving right
							if (ent->gspeed < ent->ground_top) { // If less than top speed, add accel to speed
								ent->gspeed += ent->ground_accel;
								if (ent->gspeed > ent->ground_top) { ent->gspeed = ent->ground_top; } // If we go over top after adding, cap
							}
						}
						else { // If not moving right, add decel to speed 
							ent->gspeed += ent->ground_decel;
						}
					}
					if (!ent->input->RIGHT_HOLD && ent->input->LEFT_HOLD) { // If holding left but not right
						if (ent->gspeed < 0) { // If moving left
							if (ent->gspeed > -ent->ground_top) { // If less than top speed, add accel to speed
								ent->gspeed -= ent->ground_accel;
								if (ent->gspeed < -ent->ground_top) { ent->gspeed = -ent->ground_top; } // If we go over top after adding, cap
							}
						}
						else { // If not moving right, add decel to speed 
							ent->gspeed -= ent->ground_decel;
						}
					}
				}
			}
			else {
				ent->mode = PMODE_AIR;
			}
		}

		break; // END PMODE_GROUND

	case PMODE_AIR:

		//printf("air\n");

		if (ent->grounded) {
			ent->mode = PMODE_GROUND;
		}
		else {
			if ((!ent->input->RIGHT_HOLD && !ent->input->LEFT_HOLD) || (ent->input->RIGHT_HOLD && ent->input->LEFT_HOLD)) {

				if (ent->inst->spd.x > 0) { ent->inst->spd.x -= ent->air_friction; }
				if (ent->inst->spd.x < 0) { ent->inst->spd.x += ent->air_friction; }

				if (CR_ABS(ent->inst->spd.x) <= ent->air_friction || CR_ABS(ent->inst->spd.x) <= ent->air_accel) { ent->inst->spd.x = 0; }
			}
			else {
				if (ent->input->RIGHT_HOLD && !ent->input->LEFT_HOLD) { // If holding right but not left
					if (ent->inst->spd.x > 0) { // If moving right
						if (ent->inst->spd.x < ent->air_top) { // If less than top speed, add accel to speed
							ent->inst->spd.x += ent->air_accel;
							if (ent->inst->spd.x > ent->air_top) { ent->inst->spd.x = ent->air_top; } // If we go over top after adding, cap
						}
					}
					else { // If not moving right, add decel to speed 
						ent->inst->spd.x += ent->air_decel;
					}
				}
				if (!ent->input->RIGHT_HOLD && ent->input->LEFT_HOLD) { // If holding left but not right
					if (ent->inst->spd.x < 0) { // If moving left
						if (ent->inst->spd.x > -ent->air_top) { // If less than top speed, add accel to speed
							ent->inst->spd.x -= ent->air_accel;
							if (ent->inst->spd.x < -ent->air_top) { ent->inst->spd.x = -ent->air_top; } // If we go over top after adding, cap
						}
					}
					else { // If not moving right, add decel to speed 
						ent->inst->spd.x -= ent->air_decel;
					}
				}
			}
		}

		player_jumping(ent);
		player_air_restore_angle(ent);

		break; // END PMODE_AIR
	}

	player_wallcol(ent);

	cr_camera.cams[0]->pos.x = ent->inst->pos.x - crimson.width / 2;
	cr_camera.cams[0]->pos.y = ent->inst->pos.y - crimson.height / 2;
}