#include "crimson.h"

#define CR_KEYMAP_BYTECOUNT (32)
#define CR_GAMECONFIG_SIZE	(0x91)

void crimson_run(void)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_run();
#endif

#if CR_PLATFORM == CR_ANDROID

#endif
}

void crimson_init(unsigned short width, unsigned short height, unsigned short game_width, unsigned short game_height, unsigned short hd_width, unsigned short hd_height, unsigned short game_hd_width, unsigned short game_hd_height)
{
	crimson.exitcode = 0;

	crimson.width = width;
	crimson.height = height;
	crimson.game_width = game_width;
	crimson.game_height = game_height;

	crimson.hd_width = hd_width;
	crimson.hd_height = hd_height;
	crimson.game_hd_width = game_hd_width;
	crimson.game_hd_height = game_hd_height;

	cr_debug.draw_screen = true;
	cr_drawing.target_screen = 0;

	crimson.wipe_background = true;

	cr_gameconfig_load();

#if CR_PLATFORM == CR_WINDOWS

#endif

#if CR_PLATFORM == CR_ANDROID

#endif

	crimson.cr_initialized = true;
}

void crimson_close(void)
{
	if (crimson.cr_initialized) {
#if CR_DEBUG
		printf("Closing Crimson...\n");
#endif
		// stage
		cr_stage_manager_reset();

		// objects
		cr_object_manager_close();

		if (layout.array != NULL) {
			free(layout.array);
		}

		// sprites
		cr_sprite_manager_reset(true);

		// cam
		cr_camera_manager_reset();
		free(cr_camera.cams);

		// fonts
		cr_font_reset();

		// save config
		cr_gameconfig_save();
	}
}

void cr_framework_init(const char *game_name, const char *version, void(*fr_init)(void), void(*fr_update_start)(void), void(*fr_update_end)(void), void(*fr_close)(void), float target_fps)
{
	strcpy_s(crimson.name, 0x20, game_name);
	strcpy_s(crimson.version, 0x20, version);

	crimson.fr_init = fr_init;
	crimson.fr_update_start = fr_update_start;
	crimson.fr_update_end = fr_update_end;
	crimson.fr_close = fr_close;

#if CR_DEBUG
	if (cr_debug.enabled) {
		printf("Crimson Debug: Framework Initalized:\n-Game Name: %s\n-Game Version: %s\n-Target FPS: %f\n", game_name, version, target_fps);
	}
#endif

	crimson.fr_initialized = true;
}

void cr_framework_close(void)
{
	if (crimson.fr_initialized) {
		crimson.fr_close();
	}
}

void cr_gameconfig_load(void)
{
	unsigned char *buffer;

	FILE *in_file = fopen("cd/gameconfig.crg", "rb");

	if (in_file != NULL) {
		fseek(in_file, 0, SEEK_END);
		size_t file_size = ftell(in_file);
		fseek(in_file, 0, SEEK_SET);

		buffer = malloc(CR_GAMECONFIG_SIZE);

		if (buffer != NULL) {
			fread(buffer, CR_GAMECONFIG_SIZE, 1, in_file);

			//unsigned char *p1_keymap = malloc(sizeof(unsigned char) * CR_KEYMAP_BYTECOUNT);
			int fpos = 0;

			for (int i = 0; i < 4; i++) {

				// Keyboard

				cr_input[i].keyboard_enabled = buffer[fpos++];
				cr_input[i].controller_enabled = buffer[fpos++];

				cr_input[i].A_KEYMAP = buffer[fpos++];
				cr_input[i].B_KEYMAP = buffer[fpos++];
				cr_input[i].X_KEYMAP = buffer[fpos++];
				cr_input[i].Y_KEYMAP = buffer[fpos++];

				cr_input[i].L_KEYMAP = buffer[fpos++];
				cr_input[i].R_KEYMAP = buffer[fpos++];

				cr_input[i].UP_KEYMAP = buffer[fpos++];
				cr_input[i].DOWN_KEYMAP = buffer[fpos++];
				cr_input[i].LEFT_KEYMAP = buffer[fpos++];
				cr_input[i].RIGHT_KEYMAP = buffer[fpos++];

				cr_input[i].START_KEYMAP = buffer[fpos++];
				cr_input[i].SELECT_KEYMAP = buffer[fpos++];

				// Controller 

				cr_input[i].A_CONTMAP = buffer[fpos++];
				cr_input[i].B_CONTMAP = buffer[fpos++];
				cr_input[i].X_CONTMAP = buffer[fpos++];
				cr_input[i].Y_CONTMAP = buffer[fpos++];

				cr_input[i].L_CONTMAP = buffer[fpos++];
				cr_input[i].R_CONTMAP = buffer[fpos++];

				cr_input[i].UP_CONTMAP = buffer[fpos++];
				cr_input[i].DOWN_CONTMAP = buffer[fpos++];
				cr_input[i].LEFT_CONTMAP = buffer[fpos++];
				cr_input[i].RIGHT_CONTMAP = buffer[fpos++];

				cr_input[i].START_CONTMAP = buffer[fpos++];
				cr_input[i].SELECT_CONTMAP = buffer[fpos++];

				cr_input[i].lstick_mode = buffer[fpos++];
				cr_input[i].rstick_mode = buffer[fpos++];

				cr_input[i].analog_deadzone = buffer[fpos++] * 100;
				cr_input[i].dpad_deadzone = buffer[fpos++] * 100;
			}

#if CR_PLATFORM == CR_WINDOWS
			cr_windows.current_window_state = buffer[fpos++];
			crimson.scale = buffer[fpos++];
			cr_windows.borderless = buffer[fpos++];
			cr_windows.scale_quality = buffer[fpos++];
			fpos++;//total volume
			fpos++;//music volume
			fpos++;//sfx volume
			fpos++;//voice volume

			crimson.width = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos += 2;
			crimson.height = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos += 2;
			crimson.game_width = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos += 2;
			crimson.game_height = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos += 2;

			crimson.hd_width = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos += 2;
			crimson.hd_height = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos += 2;
			crimson.game_hd_width = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos += 2;
			crimson.game_hd_height = buffer[fpos + 1] | (buffer[fpos + 0] << 8);
			fpos++;

			cr_debug.allow_devmenu = buffer[fpos++];
#endif

			cr_input_adjust_mapping();

			free(buffer);
			fclose(in_file);

			crimson.gameconfig_loaded = true;
		}
		else {
			free(buffer);
			fclose(in_file);
		}
	}
}

void cr_gameconfig_save(void)
{
	unsigned char *buffer;

	FILE *out_file = fopen("cd/gameconfig.crg", "wb");


	buffer = malloc(CR_GAMECONFIG_SIZE);

	if (buffer != NULL) {
		//function to fix input

		cr_input_restore_mapping();

		for (int i = 0; i < 4; i++) {
			fputc(cr_input[i].keyboard_enabled, out_file);
			fputc(cr_input[i].controller_enabled, out_file);

			fputc(cr_input[i].A_KEYMAP, out_file);
			fputc(cr_input[i].B_KEYMAP, out_file);
			fputc(cr_input[i].X_KEYMAP, out_file);
			fputc(cr_input[i].Y_KEYMAP, out_file);

			fputc(cr_input[i].L_KEYMAP, out_file);
			fputc(cr_input[i].R_KEYMAP, out_file);

			fputc(cr_input[i].UP_KEYMAP, out_file);
			fputc(cr_input[i].DOWN_KEYMAP, out_file);
			fputc(cr_input[i].LEFT_KEYMAP, out_file);
			fputc(cr_input[i].RIGHT_KEYMAP, out_file);

			fputc(cr_input[i].START_KEYMAP, out_file);
			fputc(cr_input[i].SELECT_KEYMAP, out_file);

			fputc(cr_input[i].A_CONTMAP, out_file);
			fputc(cr_input[i].B_CONTMAP, out_file);
			fputc(cr_input[i].X_CONTMAP, out_file);
			fputc(cr_input[i].Y_CONTMAP, out_file);

			fputc(cr_input[i].L_CONTMAP, out_file);
			fputc(cr_input[i].R_CONTMAP, out_file);

			fputc(cr_input[i].UP_CONTMAP, out_file);
			fputc(cr_input[i].DOWN_CONTMAP, out_file);
			fputc(cr_input[i].LEFT_CONTMAP, out_file);
			fputc(cr_input[i].RIGHT_CONTMAP, out_file);

			fputc(cr_input[i].START_CONTMAP, out_file);
			fputc(cr_input[i].SELECT_CONTMAP, out_file);

			fputc(cr_input[i].lstick_mode, out_file);
			fputc(cr_input[i].rstick_mode, out_file);

			int dead = cr_input[i].analog_deadzone / 100;
			fputc(dead, out_file);

			dead = cr_input[i].dpad_deadzone / 100;
			fputc(dead, out_file);
		}

#if CR_PLATFORM == CR_WINDOWS
		unsigned char b1 = 0;
		unsigned char b2 = 0;

		fputc(cr_windows.current_window_state, out_file);
		fputc(crimson.scale, out_file);
		fputc(cr_windows.borderless, out_file);
		fputc(cr_windows.scale_quality, out_file);
		fputc(100, out_file); // master
		fputc(100, out_file); // music
		fputc(100, out_file); // sfx
		fputc(100, out_file); // voice
		
		// sd
		b1 = (crimson.width >> (8)) & 0xff;
		b2 = crimson.width & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);
		b1 = (crimson.height >> (8)) & 0xff;
		b2 = crimson.height & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);

		b1 = (crimson.game_width >> (8)) & 0xff;
		b2 = crimson.game_width & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);
		b1 = (crimson.game_height >> (8)) & 0xff;
		b2 = crimson.game_height & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);

		// hd
		b1 = (crimson.hd_width >> (8)) & 0xff;
		b2 = crimson.hd_width & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);
		b1 = (crimson.hd_height >> (8)) & 0xff;
		b2 = crimson.hd_height & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);

		b1 = (crimson.game_hd_width >> (8)) & 0xff;
		b2 = crimson.game_hd_width & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);
		b1 = (crimson.game_hd_height >> (8)) & 0xff;
		b2 = crimson.game_hd_height & 0xff;
		fputc(b1, out_file);
		fputc(b2, out_file);

		fputc(cr_debug.allow_devmenu, out_file);
#endif
		
		cr_input_adjust_mapping();

		free(buffer);
		fclose(out_file);
	}
}

void cr_clear_background(void)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_clear(crimson.clear_color);
	cr_internal_queue_clear();
#endif
}