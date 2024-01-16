#include "framework.h"

bool player_jump_do(child_player* ent)
{
	if (!ent->input->A_PRESS && !ent->input->B_PRESS) { // Condition for jumping was not met, do stuff if needed and return
		
		return false;
	} 

	//If we haven't returned, then that means we need to jump. So jump!

	ent->inst->spd.x += ent->jump_factor * cr_cosf((((char)ent->angle - 0x40) * 360) / 256);
	ent->inst->spd.y += ent->jump_factor * cr_sinf((((char)ent->angle - 0x40) * 360) / 256);

	ent->jumping = true;
	ent->grounded = false;

	return true;
}

void player_jumping(child_player* ent)
{
	if (ent->jumping) {
		if (ent->inst->spd.y < -4) {
			if (!ent->input->A_HOLD && !ent->input->B_HOLD) {
				ent->inst->spd.y = -2;
			}
		}
		else {

		}
	}
}