#ifndef PLAYER_COL_H
#define PLAYER_COL_H

typedef enum {
	pcol_mode_sensor_orientations,
	pcol_mode_sensor_box,
} player_collision_modes;

typedef enum {
	pang_mode_tile,
	pang_mode_360,
} player_angle_modes;

typedef enum {
	pcp_top_left,
	pcp_top_mid,
	pcp_top_right,
	pcp_mid_up_left,
	pcp_mid_up_right,
	pcp_mid_down_left,
	pcp_mid_down_right,
	pcp_bot_left,
	pcp_bot_mid,
	pcp_bot_right,
} player_collision_points;

typedef enum {
	pcol_offset_ground_normal,
	pcol_offset_ground_duck,
} player_collision_offset_orientations;

void player_collision_update(child_player* ent);
void player_colpnt_update(child_player* ent);
void player_colpnt_set_offsets(child_player* ent);

void player_sloperesist(child_player* player);

void player_wallcol(child_player* ent);
void player_snaptofloor(child_player* ent);
void player_snaptoceil(child_player* ent);
void player_resetonfloor(child_player* ent);

#endif // !PLAYER_COL
