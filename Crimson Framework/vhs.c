#include "crimson.h"
#include "framework.h"

// ------------------------
// DECLARATIONS
// ------------------------

CR_ANIM_DATA vhs_data;

// ------------------------
// END DECLARATIONS
// ------------------------

// ------------------------
// PER ENTITY FUNCTIONS
// ------------------------

float deg = 0;

// Update code for an instance of an entity.
void vhs_update(child_vhs* ent)
{
	// Update
	ent->inst->pos.x += ent->inst->spd.x;
	ent->inst->pos.y += ent->inst->spd.y;

	ent->play_pos.x += ent->play_spd.x;
	ent->play_pos.y += ent->play_spd.y;

	ent->play_spd.y -= 0.1;
	if (ent->play_spd.y < 0) {
		ent->play_spd.y = 0;
	}

	ent->counter_pos.x += ent->counter_spd.x;
	ent->counter_pos.y += ent->counter_spd.y;

	ent->counter_spd.y += 0.1;
	if (ent->counter_spd.y > 0) {
		ent->counter_spd.y = 0;
	}

	ent->time_ms += (float)1.65;
	if (ent->time_ms >= 99) {
		ent->time_ms = 0;
		ent->time_sec++;
	}

	if (ent->time_sec >= 60) {
		ent->time_sec = 0;
		ent->time_min++;
	}

	if (ent->time_min >= 60) {
		ent->time_min = 0;
		ent->time_hour++;
	}

	// Draw
	cr_anim_display_frame(&ent->play_anim, ent->inst->pos.x + ent->play_pos.x + cr_camera.cams[0]->pos.x, ent->inst->pos.y + ent->play_pos.y + cr_camera.cams[0]->pos.y, 0, ent->inst->flip.x | (ent->inst->flip.y << 1), 6);

	//cr_anim_draw(&ent->counter_anim, ent->inst->pos.x + ent->counter_pos.x, ent->inst->pos.y + ent->counter_pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);
	vec2f t_off;
	t_off.x = crimson.width - 48;
	t_off.y = 0;

	vec2f pos;
	pos.x = ent->inst->pos.x + ent->counter_pos.x + t_off.x + cr_camera.cams[0]->pos.x;
	pos.y = ent->inst->pos.y + ent->counter_pos.y + cr_camera.cams[0]->pos.y;

	vec2f new_off;
	new_off.x = 0;
	new_off.y = 0;

	new_off.x = 0;
	if (ent->time_ms < 10) { cr_counter_draw(&ent->counter_anim, pos.x - 18, pos.y, CR_ALIGN_RIGHT, 6, 0); }
	cr_counter_draw(&ent->counter_anim, pos.x, pos.y, CR_ALIGN_RIGHT, 6, ent->time_ms);

	cr_anim_display_frame(&ent->play_anim, pos.x - new_off.x - 28, pos.y, 1, CR_NO_FLIP, 6);

	new_off.x = 48;
	if (ent->time_sec < 10) { cr_counter_draw(&ent->counter_anim, pos.x - new_off.x - 18, pos.y, CR_ALIGN_RIGHT, 6, 0); }
	cr_counter_draw(&ent->counter_anim, pos.x - new_off.x, pos.y, CR_ALIGN_RIGHT, 6, ent->time_sec);

	cr_anim_display_frame(&ent->play_anim, pos.x - new_off.x - 28, pos.y, 1, CR_NO_FLIP, 6);

	new_off.x = 96;
	if (ent->time_min < 10) { cr_counter_draw(&ent->counter_anim, pos.x - new_off.x - 18, pos.y, CR_ALIGN_RIGHT, 6, 0); }
	cr_counter_draw(&ent->counter_anim, pos.x - new_off.x, pos.y, CR_ALIGN_RIGHT, 6, ent->time_min);

	cr_anim_display_frame(&ent->play_anim, pos.x - new_off.x - 28, pos.y, 1, CR_NO_FLIP, 6);

	new_off.x = 96 + 48;
	if (ent->time_hour < 10) { cr_counter_draw(&ent->counter_anim, pos.x - new_off.x - 18, pos.y, CR_ALIGN_RIGHT, 6, 0); }
	cr_counter_draw(&ent->counter_anim, pos.x - new_off.x, pos.y, CR_ALIGN_RIGHT, 6, ent->time_hour);

	/*short vals[256];
	for (int i = 0; i < 256; i++) {
		vals[i] = 0 + ((4) * cr_cosf((deg++ * 8) - (i * 8)));
	}
	cr_anim_draw_lnscroll(&ent->counter_anim, 0, 0, vals, CR_LNSCROLL_HORIZ, 0, 6);*/
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// !! Set EVERY variable of the entity here to prevent errors from garbage values. When you add a new custom variable, set its default here. !! //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
void vhs_init(child_vhs* ent)
{
	//child_vhs* ent = &vhss[id];

	// Custom Variables
	ent->play_pos.x = 16;
	ent->play_pos.y = -64;
	ent->play_spd.x = 0;
	ent->play_spd.y = 3.8;

	ent->counter_pos.x = 16;
	ent->counter_pos.y = crimson.height + 40;
	ent->counter_spd.x = 0;
	ent->counter_spd.y = -3.8;

	ent->time_ms = 0;
	ent->time_sec = 0;
	ent->time_min = 0;
	ent->time_hour = 0;

	// Instance Variables

	cr_script_add(&vhs_data, &ent->an, 0);
	cr_script_add(&vhs_data, &ent->counter_anim, 0);
	cr_script_add(&vhs_data, &ent->play_anim, 1);
}

// ------------------------
// END PER ENTITY FUNCTIONS
// ------------------------

// ------------------------
// ONE TIME FUNCTIONS
// ------------------------

// Run every instance of the entity.
void vhs_run(void)
{
	vec2f buffer;
	buffer.x = crimson.width / 2;
	buffer.y = crimson.height / 2;
	cr_object_run(&vhs, vhs_update, NULL, NULL, -1, -1);
}

// Align entity's pointers.
void vhs_align(void)
{
	for (unsigned int id = 0; id < vhs.total_count; id++) {
		vhss[id].inst = &vhs.instance[id];
	}
}

// ------------------------
// END ONE TIME FUNCTIONS
// ------------------------

// ------------------------
// LOADING FUNCTIONS
// ------------------------

void vhs_load(const char* const filepath)
{

	// If NOT loaded, load.
	if (!vhs.loaded) { // (Load your assets in here)

		CR_ANIM dummyanim; // We use a dummy animation structure since the script loading function requires one. Just used for loading!
		cr_script_load("cd/objects/vhs/vhs.cra", &vhs_data, &dummyanim, 0);

	}
	else { // If loaded and we try to re-load, destroy all of the entitys.
		cr_destroy_all(&vhs);
	}

	// Init entity
	vhs.global = false;
	vhs.group = GROUP_HUD;
	cr_object_init(filepath, &vhs, &vhss, vhs_load, vhs_unload, vhs_init, vhs_align, vhs_run, sizeof(child_vhs), 8);

	// Mark entity as loaded
	vhs.loaded = true;
}

void vhs_unload(void)
{
	// If the entity is loaded, unload.
	if (vhs.loaded) { // (Unload your assets in here)

		cr_script_unload(&vhs_data);

		// DEALLOCATE OBJECT
		cr_object_free(&vhs);
	}

	// Mark as not loaded
	vhs.loaded = false;
}

// ------------------------
// END LOADING FUNCTIONS
// ------------------------
