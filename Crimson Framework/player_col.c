#include "framework.h"

// DATA
const unsigned char sensor_detection_types[PLAYER_ORIENTATION_COUNT][PLAYER_COLPNT_COUNT] =
{
	// pcol_offset_ground_normal
	{ 
		CR_COL_CEIL,	CR_COL_CEIL,	CR_COL_CEIL,
		CR_COL_LWALL,					CR_COL_RWALL,
		CR_COL_LWALL,					CR_COL_RWALL,
		CR_COL_FLOOR,	CR_COL_FLOOR,	CR_COL_FLOOR, 
	},

	// pcol_offset_ground_duck
	{
		CR_COL_CEIL,	CR_COL_CEIL,	CR_COL_CEIL,
		CR_COL_LWALL,					CR_COL_RWALL,
		CR_COL_LWALL,					CR_COL_RWALL,
		CR_COL_FLOOR,	CR_COL_FLOOR,	CR_COL_FLOOR,
	},
};

const vec2f sensor_orientations[PLAYER_ORIENTATION_COUNT][PLAYER_COLPNT_COUNT] =
{
	// pcol_offset_ground_normal
	{
		{ .x = -8,	.y = -20, },	// top left
		{ .x = 0,	.y = -20, },	// top mid
		{ .x = 8,	.y = -20, },	// top right

		{ .x = -10,	.y = -8, },		// mid up left
		{ .x = 10,	.y = -8, },		// mid up right

		{.x = -10,	.y = 8, },		// mid down left
		{.x = 10,	.y = 8, },		// mid down right

		{ .x = -8,	.y = 20, },		// bot left
		{ .x = 0,	.y = 20, },		// bot mid
		{ .x = 8,	.y = 20, },		// bot right
	},

	// pcol_offset_ground_duck
	{
		{ .x = -9,	.y = -6, },		// top left
		{ .x = 0,	.y = -6, },		// top mid
		{ .x = 9,	.y = -6, },		// top right

		{ .x = -9,	.y = 2, },		// mid left
		{ .x = 0,	.y = 2, },		// mid mid
		{ .x = 9,	.y = 2, },		// mid right

		{ .x = -9,	.y = 10, },		// bot left
		{ .x = 0,	.y = 10, },		// bot mid
		{ .x = 9,	.y = 10, },		// bot right
	},
};

// FUNCTIONS

void player_collision_update(child_player* ent)
{

	player_wallcol(ent);

	player_snaptoceil(ent);

	if (!ent->grounded) {
		if (ent->colpnts[pcp_bot_left].dist > 0 && ent->colpnts[pcp_bot_mid].dist > 0 && ent->colpnts[pcp_bot_right].dist > 0) {
			// do SOMETHING idk
		} 
		else {
			ent->inst->spd.y = 0;
			ent->jumping = false;
			ent->grounded = true;
			ent->landed = true;
			player_snaptofloor(ent);
		}
	}
	else {
		ent->landed = false;

		player_snaptofloor(ent);

		player_colpnt_update(ent);

		if (ent->colpnts[pcp_bot_left].dist > 0 && ent->colpnts[pcp_bot_mid].dist > 0 && ent->colpnts[pcp_bot_right].dist > 0) {
			//ent->inst->pos.y -= ent->colpnts[pcp_bot_mid].dist;
			ent->inst->spd.y = ent->grav;
			ent->grounded = false;
		}
	}
}

void player_colpnt_update(child_player* ent)
{
	for (unsigned char i = 0; i < PLAYER_COLPNT_COUNT; i++) {
		ent->colpnts[i].pos_x = ent->inst->pos.x + ent->colpnts[i].offset_x;
		ent->colpnts[i].pos_y = ent->inst->pos.y + ent->colpnts[i].offset_y;
		if (sensor_detection_types[ent->offset_orientation][i] == CR_COL_CEIL) {
			//ent->colpnts[i].pos_y ^= 0xF;
		}
		cr_colpnt_update(&ent->colpnts[i], ent->inst->col_priority, sensor_detection_types[ent->offset_orientation][i]);
	}
}

void player_colpnt_set_offsets(child_player* ent)
{
	switch (ent->offset_orientation)
	{
	case pcol_offset_ground_normal:

		for (unsigned char i = 0; i < PLAYER_COLPNT_COUNT; i++) {
			ent->colpnts[i].offset_x = sensor_orientations[ent->offset_orientation][i].x * CR_BPOS(ent->inst->flip.x);
			ent->colpnts[i].offset_y = sensor_orientations[ent->offset_orientation][i].y * CR_BPOS(ent->inst->flip.y);

			//printf("offset %d: %f, %f\n", i, sensor_orientations[ent->offset_orientation][i].x, sensor_orientations[ent->offset_orientation][i].y);
		}

		break;
	}
}

void player_wallcol(child_player* ent)
{
	short dist = 0;

	// TOP
	player_colpnt_update(ent);
	dist = cr_get_lwalldist(ent->colpnts[pcp_mid_up_left].pos_x, ent->colpnts[pcp_mid_up_left].pos_y, ent->inst->col_priority);
	if (dist < 0) {
		ent->inst->spd.x = 0;
		ent->gspeed = 0;
		ent->inst->pos.x -= dist;
	}

	player_colpnt_update(ent);
	dist = cr_get_rwalldist(ent->colpnts[pcp_mid_up_right].pos_x, ent->colpnts[pcp_mid_up_right].pos_y, ent->inst->col_priority);
	if (dist < 0) {
		ent->inst->pos.x += dist;
		ent->inst->spd.x = 0;
		ent->gspeed = 0;
	}

	// BOTTOM
	player_colpnt_update(ent);
	dist = cr_get_lwalldist(ent->colpnts[pcp_mid_down_left].pos_x, ent->colpnts[pcp_mid_down_left].pos_y, ent->inst->col_priority);
	if (dist < 0) {
		ent->inst->spd.x = 0;
		ent->gspeed = 0;
		ent->inst->pos.x -= dist;
	}

	player_colpnt_update(ent);
	dist = cr_get_rwalldist(ent->colpnts[pcp_mid_down_right].pos_x, ent->colpnts[pcp_mid_down_right].pos_y, ent->inst->col_priority);
	if (dist < 0) {
		ent->inst->pos.x += dist;
		ent->inst->spd.x = 0;
		ent->gspeed = 0;
	}
}

void player_snaptofloor(child_player* ent)
{
	int dist = ent->colpnts[pcp_bot_left].dist;
	char angle = ent->colpnts[pcp_bot_left].angle;

	if (ent->colpnts[pcp_bot_right].dist < dist) {
		dist = ent->colpnts[pcp_bot_right].dist;
		angle = ent->colpnts[pcp_bot_right].angle;
	}

	if (ent->colpnts[pcp_bot_mid].dist < dist) {
		dist = ent->colpnts[pcp_bot_mid].dist;
		angle = ent->colpnts[pcp_bot_mid].angle;
	}

	if (ent->angle_mode == pang_mode_tile) {
		ent->angle = angle;
		player_sloperesist(ent);
	}

	player_resetonfloor(ent);

	if (dist < 10) {
		ent->inst->pos.y += dist;
	}
}

void player_snaptoceil(child_player* ent)
{
	player_colpnt_update(ent);

	int dist = ent->colpnts[pcp_top_left].dist;

	if (ent->colpnts[pcp_top_right].dist < dist) {
		dist = ent->colpnts[pcp_top_right].dist;
	}

	if (ent->colpnts[pcp_top_mid].dist < dist) {
		dist = ent->colpnts[pcp_top_mid].dist;
	}
	//ent->colpnts[pcp_top_mid].
	//while (dist < 0) {
		//ent->inst->spd.y = 0;
		//ent->inst->pos.y++;
		//dist++;
	//}
	if (dist < 0) {		
		ent->inst->pos.y -= dist;
		if (ent->inst->spd.y < 0) {
			ent->inst->spd.y = 0;
		}
	}
}

void player_resetonfloor(child_player* ent)
{
	if (ent->landed) {
		if (CR_RANGE(ent->angle, 0xf0, 0xff) || CR_RANGE(ent->angle, 0x00, 0x0f)) {
			ent->gspeed = ent->inst->spd.x;
		}

		if (CR_RANGE(ent->angle, 0xe0, 0xef) || CR_RANGE(ent->angle, 0x10, 0x1f)) {
			if (CR_ABS(ent->gspeed) > ent->inst->spd.y) {
				ent->gspeed = ent->inst->spd.x;
			}
			else {
				ent->gspeed = ent->inst->spd.y * 0.5 * -CR_SIGN(cr_sin(ent->angle));
			}
		}

		if (CR_RANGE(ent->angle, 0xc0, 0xdf) || CR_RANGE(ent->angle, 0x20, 0x3f)) {
			if (CR_ABS(ent->inst->spd.x) > ent->inst->spd.y) {
				ent->gspeed = ent->inst->spd.x;
			}
			else {
				ent->gspeed = ent->inst->spd.y * -CR_SIGN(cr_sin(ent->angle));
			}
		}
	}
}

void player_sloperesist(child_player* player)
{
	if (player->angle + 0x60 < 0xC0) {
		float val = cr_sinf((player->angle * 360) / 256) * 0.125;
		if (player->gspeed > 0 && val != 0) {
			player->gspeed += val;
		}
		else if (player->gspeed < 0) {
			player->gspeed += val;
		}
	}
}