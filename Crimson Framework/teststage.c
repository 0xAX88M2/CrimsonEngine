#include "crimson.h"
#include "framework.h"

// ------------------------
// DECLARATIONS
// ------------------------

CR_MUSIC teststage_music;

CR_TEXTURE* teststage_tiles;

// ------------------------
// END DECLARATIONS
// ------------------------

float spd = 6;

void teststage_debug(void)
{

}

void teststage_objects_load(void)
{
	player_load("cd/stages/teststage/stage/players.crl");
}

void teststage_objects_unload(void)
{
	player_unload();
}

void teststage_objects_run(void)
{
	cr_objects_run(GROUP_PLAYERS);
}

void teststage_background(void)
{
	cr_stage_draw(teststage_tiles, &teststage);
}

void teststage_update(void)
{
	teststage_debug();

	teststage_objects_run();

	teststage_background();
}

void teststage_init(void)
{
	cr_camera_init(cr_camera.cams[0]);

	// Play music
	cr_music_play(teststage_music, 0);
	cr_music_volume(cr_audio.volume_music = 128);


	// Mark as initialized
	teststage.initialized = true;
}

void teststage_load(void)
{
	teststage_tiles = cr_sprite_load("cd/stages/teststage/art/teststage_tiles.png", CR_RGB_GREEN);

	cr_stage_load_info(&teststage, "cd/stages/teststage/stage/teststage.crs");

	teststage_music = cr_music_load("cd/stages/teststage/sound/teststage_music.ogg");

	teststage_objects_load();

	// Mark as loaded
	teststage.loaded = true;
}
void teststage_unload(void)
{
	cr_sprite_unload(teststage_tiles);

	cr_stage_unload_info(&teststage);

	cr_music_unload(teststage_music);

	teststage_objects_unload();

	// Mark as NOT loaded and NOT initialized
	teststage.loaded = false;
	teststage.initialized = false;
}