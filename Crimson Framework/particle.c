#include "crimson.h"
#include "framework.h"

// ------------------------
// DECLARATIONS
// ------------------------

typedef enum {
	particle_anim_heartstar,
	particle_anim_flashorb,
	particle_anim_lovelove,
} particle_anim_ids;

CR_ANIM_DATA particle_data;

// ------------------------
// END DECLARATIONS
// ------------------------

// ------------------------
// PER ENTITY FUNCTIONS
// ------------------------

bool special_set;

// Update code for an instance of an entity.
void particle_update(child_particle* ent)
{
	// Update

	// Draw
	switch (ent->inst->type)
	{
	case particle_heartstar:

		//cr_spdtopos(&ent->inst->pos, &ent->inst->spd);
		ent->inst->pos.x += ent->inst->spd.x;
		ent->inst->pos.y += ent->inst->spd.y;

		if (ent->inst->pos.y > crimson.height / 2 - 26) {
			ent->inst->spd.x = 0;
			ent->inst->spd.y = 0;
			if (!special_set) {
				special_set = true;
				special_id = ent->inst->id;
			}
			else {
				if (ent->inst->id != special_id) {
					cr_destroy(&particle, ent->inst->id);
				}
			}
		}

		if (ent->inst->id == special_id) {
			if (ent->inst->spd.y != 0) {
				if (ent->anim.frame_current > 3) {
					cr_anim_set_frame(&ent->anim, 0);
				}
			}
			cr_anim_draw(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer + 1);
		}
		else {
			cr_anim_draw(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer - 1);
		}

		break;

	case particle_flashorb:
		
		if (ent->timer-- == 0) {
			if (ent->inst->mode == 1) {
				ent->timer = 64;
				ent->frame++;
				if (ent->frame == 4) {
					ent->inst->mode++;
				}
			}

			if (ent->inst->mode == 0) {
				ent->inst->mode = 1;
				ent->timer = 64;
			}
		}

		if (ent->inst->mode != 0) {
			if (ent->flash-- == 0) {
				ent->show = !ent->show;
				ent->flash = 1;
			}
		}

		if (ent->show && ent->inst->mode != 0) {
			cr_anim_display_frame(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->frame, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);
		}

		break;

	case particle_lovelove:

		if (ent->anim.frame_current == 0 || ent->anim.frame_current == 2 || ent->anim.frame_current == 4 || ent->anim.frame_current == 6) {
			ent->anim.speed = 4;
		}
		else {
			ent->anim.speed = 2;
		}

		cr_anim_draw(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);

		break;

	case particle_loveexp1:

		cr_anim_draw(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);

		if (ent->anim.finished) {
			cr_destroy(&particle, ent->inst->id);
		}

		break;

	case particle_loveexp2:

		ent->inst->pos.x += ent->inst->spd.x;
		ent->inst->pos.y += ent->inst->spd.y;

		cr_anim_draw(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);

		if (ent->anim.finished) {
			cr_destroy(&particle, ent->inst->id);
		}

		break;

	case particle_lovetrail:

		ent->inst->pos.x += ent->inst->spd.x;
		ent->inst->pos.y += ent->inst->spd.y;

		cr_anim_draw(&ent->anim, ent->inst->pos.x, ent->inst->pos.y, ent->inst->flip.x | (ent->inst->flip.y << 1), ent->inst->layer);

		if (ent->anim.finished) {
			cr_destroy(&particle, ent->inst->id);
		}

		break;
	}
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// !! Set EVERY variable of the entity here to prevent errors from garbage values. When you add a new custom variable, set its default here. !! //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
void particle_init(child_particle* ent)
{

	// Custom Variables
	ent->frame = 0;
	ent->timer = 64;
	ent->flash = 1;
	ent->show = true;
	if (ent->inst->type == particle_anim_flashorb) {
		ent->timer = 93;
	}

	// Instance Variables

	cr_script_add(&particle_data, &ent->anim, ent->inst->type);
}

// ------------------------
// END PER ENTITY FUNCTIONS
// ------------------------

// ------------------------
// ONE TIME FUNCTIONS
// ------------------------

// Run every instance of the entity.
void particle_run(void)
{
	vec2s buffer;
	buffer.x = crimson.width / 2;
	buffer.y = crimson.height / 2;
	if (particle.loaded) { // If the entity is loaded...
		if (particle.total_count > 0) { // If entities exist...
			for (unsigned int i = 0; i < particle.total_count; i++) { // For each entity, 
				if (particle.instance[i].active == true && particle.instance[i].destroyed == false) { // If the entity is active...
					if (cr_coord_is_within(particles[i].inst->pos.x, particles[i].inst->pos.y, -buffer.x, crimson.width + buffer.x, -buffer.y, crimson.height + buffer.y, true)) {
						particle_update(&particles[i]); // Update the entity.
					}
				}
			}
		}
	}
}

// Align entity's pointers.
void particle_align(void)
{
	for (unsigned int id = 0; id < particle.total_count; id++) {
		particles[id].inst = &particle.instance[id];
	}
}

// ------------------------
// END ONE TIME FUNCTIONS
// ------------------------

// ------------------------
// LOADING FUNCTIONS
// ------------------------

void particle_load(const char* const filepath)
{

	// If NOT loaded, load.
	if (!particle.loaded) { // (Load your assets in here)

		CR_ANIM dummyanim; // We use a dummy animation structure since the script loading function requires one. Just used for loading!
		cr_script_load("cd/objects/particle/particle.cra", &particle_data, &dummyanim, 0);

	}
	else { // If loaded and we try to re-load, destroy all of the entitys.
		cr_destroy_all(&particle);
	}

	special_set = false;

	// Init entity
	particle.global = false;
	particle.group = 0;
	cr_object_init(filepath, &particle, &particles, particle_load, particle_unload, particle_init, particle_align, particle_run, sizeof(child_particle), 8);

	// Mark entity as loaded
	particle.loaded = true;
}

void particle_unload(void)
{
	// If the entity is loaded, unload.
	if (particle.loaded) { // (Unload your assets in here)

		cr_script_unload(&particle_data);

		// DEALLOCATE OBJECT
		cr_object_free(&particle);
	}

	// Mark as not loaded
	particle.loaded = false;
}

// ------------------------
// END LOADING FUNCTIONS
// ------------------------
