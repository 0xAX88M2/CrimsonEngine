#include "framework.h"

//DECLARATIONS

//Entity

//Graphics

//CR_TEXTURE *fr_fade_sprite;

CR_ANIM_DATA fr_fade_data;

//Anims


//Sound


//PER ENTITY FUNCTIONS

unsigned int fr_fade_create(unsigned char fade_type, bool fade_in, unsigned short fade_rate)
{
	unsigned int id = cr_create(&fr_fade, 0, 0, fade_type, CR_DEFAULT_LAYER_COUNT);

	fr_fades[id].fade_in = fade_in;
	if (fade_in) {
		fr_fades[id].fade = 255;
	}
	else {
		fr_fades[id].fade = 0;
	}

	fr_fades[id].rate = fade_rate;

	return id;
}

//Update code for an instance of an entity.
void fr_fade_update(child_fr_fade* ent)
{
	cr_hires_toggle(true);
	if (ent->fade_in) {
		if (ent->fade < -2) { ent->fade = -2; }
		ent->fade -= ent->rate;
	}
	else {
		if (ent->fade > 257) { ent->fade = 257; }
		ent->fade += ent->rate;
	}
	for (unsigned char screen = 0; screen <= cr_drawing.active_screens; screen++) {
		cr_screen_delegate(screen + 1);
		short cfade = ent->fade;
		if (cfade > 255) { cfade = 255; }
		if (cfade < 0) { cfade = 0; }
		cr_blending_enable(CR_BLEND_ALPHA, (unsigned char)(cfade));
		cr_anim_draw_scaled(&ent->anim, cr_camera.cams[screen]->pos.x + ent->inst->pos.x, cr_camera.cams[screen]->pos.y + ent->inst->pos.y, 0, 0, ent->inst->scale.x, ent->inst->scale.y, ent->inst->flip.x, ent->inst->layer);
		cr_blending_disable();
		cr_screen_undelegate();
	}
	cr_hires_toggle(false);
}

//Set EVERY variable of the entity here to prevent errors from garbage values. When you add a new custom variable, set its default here!
void fr_fade_init(child_fr_fade* ent)
{
	ent->inst->scale.x = crimson.game_hd_width / 8;
	ent->inst->scale.y = crimson.game_hd_height / 8;

	ent->fade = 0;
	ent->fade_in = false;

	cr_script_add(&fr_fade_data, &ent->anim, ent->inst->type);
}

//ONE TIME FUNCTIONS
//Run every instance of the entity
void fr_fade_run(void)
{
	if (fr_fade.loaded) { //If the entity is loaded...
		if (fr_fade.total_count > 0) { //If entities exist...
			for (unsigned int i = 0; i < fr_fade.total_count; i++) { //For each entity, 
				if (fr_fade.instance[i].active == true && fr_fade.instance[i].destroyed == false) { //If the entity is active...
					fr_fade_update(&fr_fades[i]); //Update the entity.
				}
			}
		}
	}
}

//Run every instance of the entity
// Align entity's pointers
void fr_fade_align(void)
{
	for (unsigned int id = 0; id < fr_fade.total_count; id++) {
		fr_fades[id].inst = &fr_fade.instance[id];
	}
}

void fr_fade_load(const char* const filepath)
{

	//If NOT loaded, load.
	if (!fr_fade.loaded) {

		//GRAPHICS
		//fr_fade_sprite = cr_sprite_load("cd/template.bmp", 0x00ff00);

		CR_ANIM dummyanim; //We use a dummy animation structure since the script loading function requires one. It gets trashed immediately, just used for loading!
		cr_script_load("cd/hud/fade.cra", &fr_fade_data, &dummyanim, 0);

		//SOUND
	}
	else { //If loaded, free entities.
		cr_destroy_all(&fr_fade);
	}

	//Init entity
	fr_fade.global = true;
	fr_fade.group = GROUP_HUD;
	cr_object_init(filepath, &fr_fade, &fr_fades, fr_fade_load, fr_fade_unload, fr_fade_init, fr_fade_align, fr_fade_run, sizeof(child_fr_fade), 8);

	//Mark entity as loaded
	fr_fade.loaded = true;
}

void fr_fade_unload(void)
{
	//Unload (run if this entity is loaded)
	if (fr_fade.loaded) {

		//GRAPHICS
		//cr_sprite_unload(fr_fade_sprite);

		cr_script_unload(&fr_fade_data);

		//SOUND

		//DEALLOCATE
		cr_destroy_all(&fr_fade);
	}

	//Do any time function is called

	//Mark as not loaded
	fr_fade.loaded = false;
}
