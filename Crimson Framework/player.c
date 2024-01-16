#include "crimson.h"
#include "framework.h"

// ------------------------
// DECLARATIONS
// ------------------------

CR_ANIM_DATA player_data;

// ------------------------
// END DECLARATIONS
// ------------------------

// ------------------------
// PER ENTITY FUNCTIONS
// ------------------------

void player_set_physics(child_player* ent, unsigned char physics_mode)
{
	ent->physics_mode = physics_mode;

	switch (ent->physics_mode)
	{
	case player_physics_normal:
		
		ent->ground_accel = 0.046875;
		ent->ground_decel = 0.5;
		ent->ground_friction = ent->ground_accel;
		ent->ground_top = 6;

		ent->air_accel = ent->ground_accel * 2;
		ent->air_decel = ent->air_accel;
		ent->air_friction = ent->air_accel / 2;
		ent->air_top = 6;

		ent->grav = 0.21875;

		ent->jump_factor = 6.0;

		break;
	}
}

// Update code for an instance of an entity.
void player_update(child_player* ent)
{
	// Update

	if (ent->grounded) {
		ent->inst->spd.x = ent->gspeed * cr_cosf(((ent->angle) * 360) / 256);
		ent->inst->spd.y = ent->gspeed * cr_sinf(((ent->angle) * 360) / 256);
		cr_spdtopos_obj(ent->inst);
	}
	else {
		cr_spdtopos_grav_obj(ent->inst, ent->grav);
	}

	player_collision_update(ent);

	player_routine_update(ent);

	ent->angle_360 = (-ent->angle * 1.40625);

	// Draw
	//cr_script_draw(&player_data, &ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);
	cr_anim_draw_scaled_rotated(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, 0, 0, ent->inst->scale.x, ent->inst->scale.y, ent->angle_360, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// !! Set EVERY variable of the entity here to prevent errors from garbage values. When you add a new custom variable, set its default here. !! //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
void player_init(child_player* ent)
{

	// CUSTOM VARIABLES

	// Collision
	player_colpnt_set_offsets(ent);
	ent->offset_orientation =	pcol_offset_ground_normal;
	ent->collision_mode =		pcol_mode_sensor_box;
	ent->angle_mode =			pang_mode_tile;

	// Statuses
	ent->routine =			PROUTINE_CONTROL;
	ent->mode =				PMODE_AIR;
	ent->action =			PACTION_NONE;
	ent->jump_flag =		0;
	ent->gspeed =			0;
	ent->angle =			0;
	ent->angle_360 =		0;
	ent->display_angle =	0;
	ent->jumping =			false;
	ent->grounded =			false;
	ent->landed =			false;

	// Physics
	player_set_physics(ent, player_physics_normal);

	// Data
	if (ent->inst->id < CR_DEFAULT_INPUT_COUNT - 1) { // If in player range, give main input
		ent->input = &cr_input[ent->inst->id];
	}
	else { // Otherwise give dummy input
		ent->input = &ent->dummyinput;
	}

	// INSTANCE VARIABLES
	ent->inst->col_priority = 0;

	// Animation
	cr_script_add(&player_data, &ent->anim, 0);

	// Hitboxes
	fr_hitbox_manager_prep(&ent->hitboxes, ent->inst);

}

void player_hitboxes(child_player *ent)
{
	if (fr_hitbox_request(&ent->hitboxes, 1)) {
		fr_hitbox_activate(ent->hitboxes.box_ids[0], -16, -16, 32, 32, HITBOX_SPECIAL, HTAG_NONE);
	}
}

// ------------------------
// END PER ENTITY FUNCTIONS
// ------------------------

// ------------------------
// ONE TIME FUNCTIONS
// ------------------------

// Run every instance of the entity.
void player_run(void)
{
	vec2f buffer;
	buffer.x = crimson.width / 2;
	buffer.y = crimson.height / 2;
	fr_hitbox_object_run(&player, player_update, NULL, NULL, player_hitboxes, offsetof(child_player, hitboxes), -1, -1);
}

// Align entity's pointers.
void player_align(void)
{
	for (unsigned int id = 0; id < player.total_count; id++) {
		players[id].inst = &player.instance[id];
	}
}

// ------------------------
// END ONE TIME FUNCTIONS
// ------------------------

// ------------------------
// LOADING FUNCTIONS
// ------------------------

void player_load(const char* const filepath)
{

	// If NOT loaded, load.
	if (!player.loaded) { // (Load your assets in here)

		CR_ANIM dummyanim; // We use a dummy animation structure since the script loading function requires one. Just used for loading!
		cr_script_load("cd/players/char1/char1.cra", &player_data, &dummyanim, 0);

	}
	else { // If loaded and we try to re-load, destroy all of the entitys.
		cr_destroy_all(&player);
	}

	// Init entity
	player.global = false;
	player.group = GROUP_PLAYERS;
	cr_object_init(filepath, &player, &players, player_load, player_unload, player_init, player_align, player_run, sizeof(child_player), 8);

	// Mark entity as loaded
	player.loaded = true;
}

void player_unload(void)
{
	// If the entity is loaded, unload.
	if (player.loaded) { // (Unload your assets in here)

		cr_script_unload(&player_data);

		// DEALLOCATE OBJECT
		cr_object_free(&player);
	}

	// Mark as not loaded
	player.loaded = false;
}

// ------------------------
// END LOADING FUNCTIONS
// ------------------------
