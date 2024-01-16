#include "crimson.h"
#include "framework.h"

// ------------------------
// DECLARATIONS
// ------------------------

// ASSETS

typedef enum {
	vape_gfx_grid,
	vape_gfx_scan,
	vape_gfx_sun,
	vape_gfx_palm,
} VAPE_GFX;

CR_TEXTURE *vape_gfx[10];

CR_MUSIC vape_music;

// STAGE
float vape_grid_glow = 0;
vec2f vape_grid_off;

vec2f vape_tree_off;
vec2f vape_tree_spd;
vec2f vape_tree_prop;

float tree_deg = 0;
float tree_deg_spd = 128;

// ------------------------
// END DECLARATIONS
// ------------------------

void vape_objects_run(void)
{
	cr_objects_run(GROUP_GLOBAL);
	cr_objects_run(GROUP_HUD);
	cr_objects_run(GROUP_GENERIC);
}

void vape_background(void)
{

	vape_grid_off.x -= 1;
	vape_grid_off.y += 1;

	vape_grid_glow++;

	if (vape_grid_glow >= 360) {
		vape_grid_glow = 0;
	}
	float glow = (float)(316 - (16 * cr_sinf((int)vape_grid_glow * 1)));

	cr_lnscroll_wave(vape_grid_glow, 0, 4, 2, 16);
	/*for (int i = 0; i < 256; i++) {
		vals[i] = root + ((width) * cr_cosf((vape_grid_glow * speed) - (i * fuck)));
	}*/

	cr_blending_enable(CR_BLEND_ALPHA, (unsigned char)((glow) * 2.8125));
	//cr_sprite_draw(vape_gfx[vape_gfx_sun], (crimson.width / 2) - 128, (crimson.height / 2) - 128, CR_NO_FLIP, 1);
	cr_sprite_draw_lnscroll(vape_gfx[vape_gfx_sun], (crimson.width / 2) - 128, (crimson.height / 2) - 128, cr_lnscroll, CR_LNSCROLL_HORIZ, CR_NO_FLIP, 1);
	//cr_section_draw_lnscroll(vape_gfx[vape_gfx_sun], (crimson.width / 2) - 128, (crimson.height / 2) - 128, 0, 0, 128, 128, cr_lnscroll, CR_LNSCROLL_VERTI, CR_NO_FLIP, 1);
	cr_parallax_draw(vape_gfx[vape_gfx_scan], vape_grid_off.x, vape_grid_off.y - 2, 0, 0, CR_BG_H | CR_BG_V, CR_NO_FLIP, 1);
	cr_blending_disable();

	cr_blending_enable(CR_BLEND_ALPHA, (unsigned char)((glow) * 2.8125));
	cr_parallax_draw(vape_gfx[vape_gfx_grid], vape_grid_off.x, vape_grid_off.y, 0, 0, CR_BG_H | CR_BG_V, CR_NO_FLIP, 0);
	cr_blending_disable();

	vape_tree_off.x += vape_tree_spd.x;
	vape_tree_off.y += vape_tree_spd.y;

	vape_tree_spd.y += 0.075;
	if (vape_tree_spd.y > 0) {
		vape_tree_spd.y = 0;
	}

	vape_tree_spd.x -= 0.075;
	if (vape_tree_spd.x < 1) {
		vape_tree_spd.x = 1;
	}

	//cr_parallax_draw(vape_gfx[vape_gfx_palm], vape_tree_off.x, vape_tree_off.y, 0, 0, CR_BG_H, CR_NO_FLIP, 4);
	vape_tree_prop.y--;
	if (cr_input->RIGHT_PRESS) {
		vape_tree_prop.y += 1;
	}
	if (cr_input->LEFT_PRESS) {
		vape_tree_prop.y -= 1;
	}
	cr_lnscroll_wave(tree_deg, 0, 1, vape_tree_prop.x, 2.5);
	tree_deg -= tree_deg_spd;
	tree_deg_spd -= 0.25;
	if (tree_deg_spd < 1) {
		tree_deg_spd = 1;
	}
	if (vape_tree_prop.x > 8) {
		vape_tree_prop.x -= 0.5;
	}

	cr_parallax_draw_lnscroll(vape_gfx[vape_gfx_palm], vape_tree_off.x, vape_tree_off.y, 0, 0, CR_BG_H, cr_lnscroll, CR_LNSCROLL_HORIZ, 0, 5);
	//cr_sprite_draw_lnscroll(vape_gfx[vape_gfx_palm], vape_tree_off.x, vape_tree_off.y, vals, CR_LNSCROLL_HORIZ, CR_NO_FLIP, 4);

	//cr_anim_draw_lnscroll(&vhss[0].an, 128, 128, vals, CR_LNSCROLL_HORIZ, 0, 6);
	//cr_parallax_draw_animated(vape_gfx[vape_gfx_grid], 128, 128, 0, 0, CR_BG_H, 0, 6);
	//cr_parallax_draw_lnscroll(vape_gfx[vape_gfx_grid], 128, 128, 0, 0, CR_BG_H, vals, CR_LNSCROLL_VERTI, 0, 6);
	//cr_anim_draw(&vhss[0].an, 0, 0, 0, 6);
}

void vape_update(void)
{
	if (cr_input->UP_HOLD) {
		cr_camera.cams[0]->pos.y -= 8;
	}

	if (cr_input->DOWN_HOLD) {
		cr_camera.cams[0]->pos.y += 8;
	}

	if (cr_input->RIGHT_HOLD) {
		cr_camera.cams[0]->pos.x += 8;
	}

	if (cr_input->LEFT_HOLD) {
		cr_camera.cams[0]->pos.x -= 8;
	}

	cr_camera_update();

	vape_objects_run();
	vape_background();

	if (cr_input->A_PRESS) {
		cr_create(&helios, 0, 0, 0, 4);
		child_helios* ent = cr_objects.last_spawned;
		ent->inst->pos.x = 128;
		printf("pos x: %f\n", ent->inst->pos.x);
	}
}

void vape_init(void)
{
	vape_grid_glow = 0;
	vape_grid_off.x = 0;
	vape_grid_off.y = 0;

	vape_tree_off.x = 0;
	vape_tree_off.y = crimson.height;
	vape_tree_spd.x = 12;
	vape_tree_spd.y = -6;

	vape_tree_prop.x = 64;
	vape_tree_prop.y = 64;

	// Play music
	cr_music_play(vape_music, 0);
	cr_music_volume(cr_audio.volume_music = 128);

	tree_deg = 0;
	tree_deg_spd = 32;

	cr_create(&vhs, 0, 0, 0, 3);

	unsigned int id = 0;
	id = cr_create(&helios, 0, 0, 0, 4);
	helioss[id].degree.x = 0;
	helioss[id].degree.y = 0;

	id = cr_create(&helios, 0, 0, 0, 4);
	helioss[id].degree.x = 120;
	helioss[id].degree.y = 120;

	id = cr_create(&helios, 0, 0, 0, 4);
	helioss[id].degree.x = 240;
	helioss[id].degree.y = 240;

	fr_fade_create(FR_FADE_BLACK, FR_FADE_IN, 8);

	cr_camera.cams[0]->pos.x = 0;
	cr_camera.cams[0]->pos.y = 0;

	// Mark as initialized
	vape.initialized = true;
}

void vape_load(void)
{
	// GFX
	vape_gfx[vape_gfx_grid] = cr_sprite_load("cd/stages/vape/grid.png", CR_RGB_GREEN);
	vape_gfx[vape_gfx_scan] = cr_sprite_load("cd/stages/vape/scan.png", CR_RGB_GREEN);
	vape_gfx[vape_gfx_sun] = cr_sprite_load("cd/stages/vape/sun.png", CR_RGB_GREEN);
	vape_gfx[vape_gfx_palm] = cr_sprite_load("cd/stages/vape/palm.png", CR_RGB_MAGENTA);

	// SOUND
	vape_music = cr_music_load("cd/sound/music/botanic.ogg");

	// OBJECTS
	vhs_load(NULL);
	helios_load(NULL);

	// Mark as loaded
	vape.loaded = true;
}
void vape_unload(void)
{
	// GFX
	cr_sprite_unload(vape_gfx[vape_gfx_grid]);
	cr_sprite_unload(vape_gfx[vape_gfx_scan]);
	cr_sprite_unload(vape_gfx[vape_gfx_sun]);
	cr_sprite_unload(vape_gfx[vape_gfx_palm]);

	// SOUND
	cr_music_unload(vape_music);

	// OBJECTS
	vhs_unload();
	helios_unload();


	// Mark as NOT loaded and NOT initialized
	vape.loaded = false;
	vape.initialized = false;
}
