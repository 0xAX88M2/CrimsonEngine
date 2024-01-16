#include "crimson.h"
#include "framework.h"

// ------------------------
// DECLARATIONS
// ------------------------

typedef enum {
	star_loc_middle,
	star_loc_far_left,
	star_loc_far_right,
	star_loc_mid_left,
	star_loc_mid_right,
} heartstar_locations;

typedef enum {
	bg_back,
	bg_level6,
	bg_hyperzone,
	bg_clouds,
	bg_castle,
	bg_shutter,
} level6_bg_sprites;

typedef enum {
	sfx_lovelove,
	sfx_heartstar,
	sfx_jump,
	sfx_rise,
} level6_sounds;

CR_TEXTURE* level6_bg[6];
CR_TEXTURE* level6_fg[6];

CR_MUSIC level6_music;
CR_SOUND level6_sfx[4];

float level6_cloud_off[4];
float level6_shutter_off[2];

vec2f kirby_pos;
vec2f kirby_spd;
unsigned char kirby_mode;
unsigned char kirby_timer;
bool kirby_rise;

bool friends_look;

CR_ANIM_DATA kirby_data;
CR_ANIM kirby_anim;

int heart_timer;
unsigned char heart_mode;
short heart_count;
short heart_sfxtimer;
short heart_sfxcount;

unsigned int lovelove_id;

unsigned char startimer;

unsigned char dir_types[20] =
{
	star_loc_mid_left,
	star_loc_mid_right,
	star_loc_far_left,
	star_loc_far_right,
	star_loc_far_left,
	star_loc_far_right,
	star_loc_middle,

	star_loc_far_right,
	star_loc_mid_left,
	star_loc_far_right,
	star_loc_far_left,
	star_loc_middle,

	star_loc_far_left,
	star_loc_mid_right,
	star_loc_mid_left,
	star_loc_far_left,
	star_loc_far_right,
	star_loc_middle,
	star_loc_far_right,
	star_loc_far_left,
};

// ------------------------
// END DECLARATIONS
// ------------------------

void level6_background(void)
{
	cr_parallax_draw(level6_bg[bg_back], 0, 0, 0, 0, CR_BG_V | CR_BG_H, CR_NO_FLIP, 0);

	cr_parallax_draw_section(level6_bg[bg_level6], 0, 0, 0, 0, 224, 0, 32, 47, CR_BG_H, CR_NO_FLIP, 6);
	cr_sprite_draw(level6_bg[bg_level6], 0, 0, CR_NO_FLIP, 6);

	cr_parallax_draw_section(level6_bg[bg_hyperzone], 0, crimson.height - 57, 0, 0, 0, 0, 32, 57, CR_BG_H, CR_NO_FLIP, 6);
	cr_sprite_draw(level6_bg[bg_hyperzone], crimson.width - 224, crimson.height - 57, CR_NO_FLIP, 6);

	cr_parallax_draw_section(level6_bg[bg_clouds], level6_cloud_off[0] - 45, 16, 0, 0, 0, 0, 256, 32, CR_BG_H, CR_NO_FLIP, 2);
	cr_parallax_draw_section(level6_bg[bg_clouds], level6_cloud_off[1] + 45 + 83 + 38, 16 + 32, 0, 0, 0, 32, 256, 20, CR_BG_H, CR_NO_FLIP, 2);
	cr_parallax_draw_section(level6_bg[bg_clouds], level6_cloud_off[2] - 45, 16 + 32 + 20, 0, 0, 0, 32 + 20, 256, 16, CR_BG_H, CR_NO_FLIP, 2);
	cr_parallax_draw_section(level6_bg[bg_clouds], level6_cloud_off[3] - 45 + 22, 16 + 32 + 20 + 16, 0, 0, 0, 32 + 20 + 16, 256, 12, CR_BG_H, CR_NO_FLIP, 2);

	level6_cloud_off[1] -= 2;
	level6_cloud_off[2] -= 1;
	level6_cloud_off[3] -= 0.5;

	cr_sprite_draw(level6_bg[bg_castle], crimson.width / 2 - (256 / 2), crimson.height - 57 - 64, CR_NO_FLIP, 3);

	if (!friends_look) {
		cr_sprite_draw(level6_fg[0], crimson.width / 2 - (256 / 2), 0, CR_NO_FLIP, 4);
	}
	else {
		cr_sprite_draw(level6_fg[1], crimson.width / 2 - (256 / 2), 0, CR_NO_FLIP, 4);
	}

	for (unsigned char i = 0; i < (crimson.height / 2) / 8; i++) {
		cr_parallax_draw(level6_bg[bg_shutter], 0, level6_shutter_off[0] + (i * 8), 0, 0, CR_BG_H, CR_NO_FLIP, 7);
		cr_parallax_draw(level6_bg[bg_shutter], 0, (level6_shutter_off[1] + crimson.height / 2) + (i * 8), 0, 0, CR_BG_H, CR_NO_FLIP, 7);
	}

	if (kirby_mode != 6) {
		level6_shutter_off[0] -= 4;
		level6_shutter_off[1] += 4;
	}
	else {

		cr_music_volume(cr_audio.volume_music -= 2);

		level6_shutter_off[0] += 4;
		level6_shutter_off[1] -= 4;

		if (level6_shutter_off[0] > 0) {
			level6_shutter_off[0] = 0;
		}
		if (level6_shutter_off[1] < 0) {
			level6_shutter_off[1] = 0;
		}
	}
}

void level6_hearstars(void)
{
	if (heart_timer-- == 0) {
		if (heart_mode == 2) {
			cr_sound_channel_play(level6_sfx[sfx_lovelove], 0, 0);
			cr_destroy_all(&particle);
			cr_radial_spawn(&particle, crimson.width / 2, crimson.height / 2 - 25, 0, 32, 1, 0, 0, 3, 3, 0, particle_loveexp2, 5);
			cr_create(&particle, crimson.width / 2, crimson.height / 2 - 25, particle_loveexp1, 5);
			lovelove_id = cr_create(&particle, crimson.width / 2, crimson.height / 2 - 25, particle_lovelove, 5);
			heart_mode++;

			kirby_mode++;
			kirby_timer = 120;
		}

		if (heart_mode == 1) {
			heart_timer = 15;
			unsigned int id = 0;
			if (heart_count < 20) {

				switch (dir_types[heart_count])
				{
				case star_loc_middle:
					id = cr_create(&particle, crimson.width / 2, 40, particle_heartstar, 5);
					particles[id].inst->spd.y = 1;
					particles[id].inst->spd.x = 0;
					break;

				case star_loc_mid_left:
					id = cr_create(&particle, crimson.width / 2 - 32, 40, particle_heartstar, 5);
					particles[id].inst->spd.y = 1.05;
					particles[id].inst->spd.x = 0.75;
					break;
				case star_loc_mid_right:
					id = cr_create(&particle, crimson.width / 2 + 32, 40, particle_heartstar, 5);
					particles[id].inst->spd.y = 1.05;
					particles[id].inst->spd.x = -0.75;
					break;

				case star_loc_far_left:
					id = cr_create(&particle, crimson.width / 2 - 95, 40, particle_heartstar, 5);
					particles[id].inst->spd.y = 0.8;
					particles[id].inst->spd.x = 1.6;
					break;
				case star_loc_far_right:
					id = cr_create(&particle, crimson.width / 2 + 95, 40, particle_heartstar, 5);
					particles[id].inst->spd.y = 0.8;
					particles[id].inst->spd.x = -1.6;
					break;
				}

				cr_anim_set_frame(&particles[id].anim, 4);
				particles[id].anim.frame_timer = particles[special_id].anim.frame_timer;
				heart_count++;

				if (heart_count == 20) {
					heart_mode++;
					heart_timer = 78;
				}
			}
		}

		if (heart_mode == 0) {
			heart_mode++;
			heart_timer = 8;
		}
	}

	if (heart_sfxcount < 9) {
		if (heart_sfxtimer-- == 0) {
			cr_sound_channel_play(level6_sfx[sfx_heartstar], 0, 0);
			heart_sfxtimer = 35;
			heart_sfxcount++;
		}
	}
}

void level6_kirby(void)
{
	unsigned int id = 0;

	if (kirby_timer > 0) {
		kirby_timer--;
	}
	else {
		switch (kirby_mode)
		{
		case 0:

			break;

		case 1:

			cr_script_add(&kirby_data, &kirby_anim, 1);

			kirby_timer = 9;
			kirby_mode++;

			break;

		case 2:

			cr_sound_play(level6_sfx[sfx_jump], 0);
			kirby_spd.y = -2;
			kirby_mode++;

			break;

		case 4:

			kirby_spd.y = 1.5;
			kirby_mode++;
			kirby_timer = 16;

			break;

		case 5:

			id = cr_create(&particle, kirby_pos.x, kirby_pos.y, particle_lovetrail, 4);
			kirby_timer = 4;
			particles[id].inst->spd.y = 1;

			break;
		}
	}

	kirby_pos.x += kirby_spd.x;
	kirby_pos.y += kirby_spd.y;

	if (kirby_mode == 4 || kirby_mode == 5) {
		kirby_spd.y -= 0.1;

		if (kirby_spd.y < 0 && !kirby_rise && kirby_mode == 5) {
			cr_sound_play(level6_sfx[sfx_rise], 0);
			kirby_rise = true;
		}

		if (kirby_spd.y < -1) {
			friends_look = true;
		}


		if (kirby_pos.y < -512) {
			printf("uwu\n");
			level6_shutter_off[0] = -(crimson.height / 2);
			level6_shutter_off[1] = crimson.height / 2;
			kirby_mode = 6;
		}
	}
	else {
		if (kirby_mode == 3) {
			kirby_spd.y += 0.1;
		}
	}

	cr_anim_draw(&kirby_anim, kirby_pos.x, kirby_pos.y, CR_NO_FLIP, 4);

	if (particles == NULL || particles[lovelove_id].inst == NULL) {
		return;
	}

	if (kirby_pos.y < crimson.height / 2 - 25 + 31 && kirby_mode == 3) {
		kirby_pos.y = crimson.height / 2 - 25 + 31;
		kirby_spd.y = 0;
		kirby_timer = 1;
		kirby_mode++;
		cr_destroy_all(&particle);
		cr_script_add(&kirby_data, &kirby_anim, 2);
	}
}

void level6_update(void)
{
	if (startimer > 0) {
		startimer--;

		if (startimer == 0) {
			cr_sound_play(level6_sfx[sfx_heartstar], 0);
		}
	}
	else {
		level6_background();

		particle_run();

		level6_hearstars();
		level6_kirby();
	}
}

void level6_init(void)
{
	level6_cloud_off[0] = 0;
	level6_cloud_off[1] = 0;
	level6_cloud_off[2] = 0;
	level6_cloud_off[3] = 0;

	level6_shutter_off[0] = 3;
	level6_shutter_off[1] = -4;

	kirby_pos.x = crimson.width / 2;
	kirby_pos.y = crimson.height - 57 - 32;

	kirby_spd.x = 0;
	kirby_spd.y = 0;

	kirby_mode = 0;
	kirby_timer = 120;
	kirby_rise = false;

	friends_look = false;

	heart_timer = 40;
	heart_mode = 0;
	heart_count = 0;
	heart_sfxtimer = 60;
	heart_sfxcount = 0;

	startimer = 30;

	cr_create(&particle, crimson.width / 2, crimson.height / 2 - 25, particle_flashorb, 5);

	special_id = cr_create(&particle, crimson.width / 2, 38, particle_heartstar, 5);
	particles[special_id].inst->spd.y = 1;

	// Play music
	cr_music_play(level6_music, 0);
	cr_music_volume(cr_audio.volume_music = 128);

	// Mark as initialized
	level6.initialized = true;
}

void level6_load(void)
{
	level6_bg[bg_back] = cr_sprite_load("cd/stages/level6/bg_back.png", CR_RGB_GREEN);
	level6_bg[bg_level6] = cr_sprite_load("cd/stages/level6/bg_level6.png", CR_RGB_GREEN);
	level6_bg[bg_hyperzone] = cr_sprite_load("cd/stages/level6/bg_hyperzone.png", CR_RGB_GREEN);
	level6_bg[bg_clouds] = cr_sprite_load("cd/stages/level6/bg_clouds.png", CR_RGB_GREEN);
	level6_bg[bg_castle] = cr_sprite_load("cd/stages/level6/bg_castle.png", CR_RGB_GREEN);
	level6_bg[bg_shutter] = cr_sprite_load("cd/stages/level6/bg_shutter.png", CR_RGB_GREEN);

	level6_fg[0] = cr_sprite_load("cd/stages/level6/fg_friends1.png", CR_RGB_GREEN);
	level6_fg[1] = cr_sprite_load("cd/stages/level6/fg_friends2.png", CR_RGB_GREEN);

	level6_music = cr_music_load("cd/stages/level6/level6_music.ogg");

	level6_sfx[sfx_lovelove] = cr_sound_load("cd/stages/level6/sfx_lovelove.wav");
	level6_sfx[sfx_heartstar] = cr_sound_load("cd/stages/level6/sfx_heartstar.wav");
	level6_sfx[sfx_jump] = cr_sound_load("cd/stages/level6/sfx_jump.wav");
	level6_sfx[sfx_rise] = cr_sound_load("cd/stages/level6/sfx_rise.wav");

	cr_script_load("cd/players/kirby/kirby.cra", &kirby_data, &kirby_anim, 0);

	particle_load(NULL);

	// Mark as loaded
	level6.loaded = true;
}
void level6_unload(void)
{
	cr_sprite_unload(level6_bg[bg_back]);
	cr_sprite_unload(level6_bg[bg_level6]);
	cr_sprite_unload(level6_bg[bg_hyperzone]);
	cr_sprite_unload(level6_bg[bg_clouds]);
	cr_sprite_unload(level6_bg[bg_castle]);
	cr_sprite_unload(level6_bg[bg_shutter]);

	cr_sprite_unload(level6_fg[0]);
	cr_sprite_unload(level6_fg[1]);

	cr_music_unload(level6_music);

	cr_sound_unload(level6_sfx[sfx_lovelove]);
	cr_sound_unload(level6_sfx[sfx_heartstar]);
	cr_sound_unload(level6_sfx[sfx_jump]);
	cr_sound_unload(level6_sfx[sfx_rise]);

	cr_script_unload(&kirby_data);

	particle_unload();

	// Mark as NOT loaded and NOT initialized
	level6.loaded = false;
	level6.initialized = false;
}