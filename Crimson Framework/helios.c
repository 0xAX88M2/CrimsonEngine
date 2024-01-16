#include "crimson.h"
#include "framework.h"

// ------------------------
// DECLARATIONS
// ------------------------

//CR_ANIM_DATA helios_data;

CR_TEXTURE* helios_sprite;

// ------------------------
// END DECLARATIONS
// ------------------------

// ------------------------
// PER ENTITY FUNCTIONS
// ------------------------

void helios_screen_enter(child_helios* ent)
{

}

// Update code for an instance of an entity.
void helios_update(child_helios* ent)
{
	// Update
	ent->alpha++;

	//if (ent->alpha >= 360) {
	//	ent->alpha = 0;
	//}
	//float glow = (float)(316 - (32 * cr_sinf((int)ent->alpha * 1)));

	ent->alpha += 2;
	int glow = 128 - (64 * cr_sinf(ent->alpha));
	if (glow < 0) { glow = 0; }
	if (glow > 255) { glow = 255; }

	ent->degree.x += 1;
	ent->degree.y += 1;

	if (ent->degree.x > 360) { ent->degree.x -= 360; }
	if (ent->degree.y > 360) { ent->degree.y -= 360; }

	vec2f root;
	root.x = crimson.width / 2;
	root.y = crimson.height / 2;

	ent->radius += 0.5;
	float radius = 128 - (128 * cr_sinf(ent->radius * 2));
	//if (radius < 0) { radius = 0; }
	//if (radius > 255) { radius = 255; }

	vec2f pos;
	pos.x = (root.x - 0) + ((radius / 2) * cr_sinf(ent->degree.x));
	pos.y = (root.y - 0) - ((radius / 2) * cr_cosf(ent->degree.y));

	// Draw
	cr_sprite_draw_scaled_rotated(helios_sprite, pos.x, pos.y, 118, 228, (radius / 512) + 0.5, (radius / 512) + 0.5, ent->degree.x * -1, CR_NO_FLIP, ent->inst->layer);
	cr_blending_enable(CR_BLEND_ADD, glow);
	cr_sprite_draw_scaled_rotated(helios_sprite, pos.x, pos.y, 118, 228, (radius / 512) + 0.5, (radius / 512) + 0.5, ent->degree.x * -1, CR_NO_FLIP, ent->inst->layer);
	cr_blending_disable();
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
// !! Set EVERY variable of the entity here to prevent errors from garbage values. When you add a new custom variable, set its default here. !! //
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
void helios_init(child_helios* ent)
{
	//child_helios* ent = &helioss[id];

	// Custom Variables
	ent->alpha = 0;

	ent->degree.x = 0;
	ent->degree.y = 0;

	ent->radius = 0;

	// Instance Variables

	//cr_script_add(&helios_data, &ent->anim, 0);
}

// ------------------------
// END PER ENTITY FUNCTIONS
// ------------------------

// ------------------------
// ONE TIME FUNCTIONS
// ------------------------

// Run every instance of the entity.
void helios_run(void)
{
	vec2f buffer;
	buffer.x = crimson.width * 2;
	buffer.y = crimson.height * 2;
	cr_object_run(&helios, helios_update, helios_screen_enter, NULL, buffer.x, buffer.y);
}

// Align entity's pointers.
void helios_align(void)
{
	for (unsigned int id = 0; id < helios.total_count; id++) {
		helioss[id].inst = &helios.instance[id];
	}
}

// ------------------------
// END ONE TIME FUNCTIONS
// ------------------------

// ------------------------
// LOADING FUNCTIONS
// ------------------------

void helios_load(const char* const filepath)
{

	// If NOT loaded, load.
	if (!helios.loaded) { // (Load your assets in here)

		//CR_ANIM dummyanim; // We use a dummy animation structure since the script loading function requires one. Just used for loading!
		//cr_script_load("cd/template.cra", &helios_data, &dummyanim, 0);

		helios_sprite = cr_sprite_load("cd/objects/helios/helios.png", CR_RGBA_BLANK);

	}
	else { // If loaded and we try to re-load, destroy all of the entitys.
		cr_destroy_all(&helios);
	}

	// Init entity
	helios.global = false;
	helios.group = GROUP_GENERIC;
	cr_object_init(filepath, &helios, &helioss, helios_load, helios_unload, helios_init, helios_align, helios_run, sizeof(child_helios), 8);

	// Mark entity as loaded
	helios.loaded = true;
}

void helios_unload(void)
{
	// If the entity is loaded, unload.
	if (helios.loaded) { // (Unload your assets in here)

		//cr_script_unload(&helios_data);

		cr_sprite_unload(helios_sprite);

		// DEALLOCATE OBJECT
		cr_object_free(&helios);
	}

	// Mark as not loaded
	helios.loaded = false;
}

// ------------------------
// END LOADING FUNCTIONS
// ------------------------
