#include "crimson.h"
#include "cr_win_debug.h"

typedef enum {
	DEVMENU_STAGE,
	DEVMENU_GAMECONFIG,
	DEVMENU_DEBUG_OPTIONS,
	DEVMENU_EXITGAME,
} CR_DEVEMNU_SELECTIONS;

typedef enum {
	MODE_MAIN,
	MODE_STAGELIST,
	MODE_GAMECONFIG,
	MODE_DEBUGOPTIONS,
} CR_DEVEMNU_MODES;

void cr_windows_devmenu_run(void)
{
	cr_devmenu.mainmenu_item_count = 3;
	if (cr_devmenu.active) {

		//OH BOY HERE WE GO DOIn' SOME DUMB SHIT

		crimson.master_pause = true;

		int ver_spacing = cr_devmenu.white.width + (strlen(crimson.name) * cr_devmenu.white.width) + cr_devmenu.white.width;
		int fontwidth = cr_devmenu.white.width;
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 8, CR_ALIGN_LEFT, 7, "Crimson Engine Dev Menu", false);
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 18, CR_ALIGN_LEFT, 7, "-----------------------", false);
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + (fontwidth * 1), cr_camera.cams[0]->pos.y + crimson.game_height - (fontwidth * 2), CR_ALIGN_LEFT, 7, crimson.name, false);
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + crimson.game_width - (fontwidth * 2), cr_camera.cams[0]->pos.y + crimson.game_height - (fontwidth * 2), CR_ALIGN_RIGHT, 7, crimson.version, false);

		if (cr_debug.game_debug) { cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + (crimson.game_width - (fontwidth * 19)), cr_camera.cams[0]->pos.y + 8, CR_ALIGN_LEFT, 7, "F3: Game Debug-On", false); }
		else { cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + (crimson.game_width - (fontwidth * 19)), cr_camera.cams[0]->pos.y + 8, CR_ALIGN_LEFT, 7, "F3: Game Debug-Off", false); }
		if (cr_windows.is_fullscreen) { cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + (crimson.game_width - (fontwidth * 19)), cr_camera.cams[0]->pos.y + 18, CR_ALIGN_LEFT, 7, "F4: Go Windowed", false); }
		else { cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + (crimson.game_width - (fontwidth * 19)), cr_camera.cams[0]->pos.y + 18, CR_ALIGN_LEFT, 7, "F4: Go Fullscreen", false); }
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + crimson.game_width - (fontwidth * 19), cr_camera.cams[0]->pos.y + 28, CR_ALIGN_LEFT, 7, "F5: Restart Game", false);
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + crimson.game_width - (fontwidth * 19), cr_camera.cams[0]->pos.y + 38, CR_ALIGN_LEFT, 7, "F6: Restart Stage", false);
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + crimson.game_width - (fontwidth * 19), cr_camera.cams[0]->pos.y + 48, CR_ALIGN_LEFT, 7, "F7: Step frame", false);
		cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + crimson.game_width - (fontwidth * 19), cr_camera.cams[0]->pos.y + 58, CR_ALIGN_LEFT, 7, "F8: Fast forward", false);

		switch (cr_devmenu.mode)
		{
		case MODE_MAIN:

			if (cr_input[CR_P1].UP_PRESS) {
				cr_devmenu.selection--;
				if (cr_devmenu.selection < 0) {
					cr_devmenu.selection = cr_devmenu.mainmenu_item_count;
				}
			}
			if (cr_input[CR_P1].DOWN_PRESS) {
				cr_devmenu.selection++;
				if (cr_devmenu.selection > cr_devmenu.mainmenu_item_count) {
					cr_devmenu.selection = 0;
				}
			}

			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 28, CR_ALIGN_LEFT, 7, "Stage List", false);
			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 48, CR_ALIGN_LEFT, 7, "Gameconfig", false);
			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 68, CR_ALIGN_LEFT, 7, "Debug Options", false);
			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 88, CR_ALIGN_LEFT, 7, "Exit Game", false);

			switch (cr_devmenu.selection)
			{
			case 0:
				cr_spritefont_draw(&cr_devmenu.red, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 28, CR_ALIGN_LEFT, 7, "Stage List", false);
				break;
			case 1:
				cr_spritefont_draw(&cr_devmenu.red, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 48, CR_ALIGN_LEFT, 7, "Gameconfig", false);
				break;
			case 2:
				cr_spritefont_draw(&cr_devmenu.red, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 68, CR_ALIGN_LEFT, 7, "Debug Options", false);
				break;
			case 3:
				cr_spritefont_draw(&cr_devmenu.red, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 88, CR_ALIGN_LEFT, 7, "Exit Game", false);
				break;
			}

			if (cr_input[CR_P1].START_PRESS) {
				switch (cr_devmenu.selection)
				{
				case DEVMENU_STAGE:
					cr_devmenu.mode = MODE_STAGELIST;
					//cr_devmenu.selection = cr_stage.current->id - 1;
					cr_devmenu.selection = 0;
					cr_debug.step = true;
					break;
				case DEVMENU_GAMECONFIG:
					cr_devmenu.mode = MODE_GAMECONFIG;
					cr_devmenu.selection = 0;
					cr_debug.step = true;
					break;

				case DEVMENU_DEBUG_OPTIONS:
					cr_devmenu.mode = MODE_DEBUGOPTIONS;
					cr_devmenu.selection = 0;
					cr_debug.step = true;
					break;

				case DEVMENU_EXITGAME:
					crimson.exitcode = 1;
					//cr_debug.step = true;
					break;
				}
			}

			break; //Mode main

		case MODE_STAGELIST:

			if (cr_input[CR_P1].UP_PRESS) {
				cr_devmenu.selection--;
				if (cr_devmenu.selection < 0) {
					cr_devmenu.selection = cr_stage.stage_count - 1;
				}
			}
			if (cr_input[CR_P1].DOWN_PRESS) {
				cr_devmenu.selection++;
				if (cr_devmenu.selection >= cr_stage.stage_count) {
					cr_devmenu.selection = 0;
				}
			}

			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 28, CR_ALIGN_LEFT, 7, "Stage List:", false);

			for (int i = 0; i < cr_stage.stage_count; i++) {
				cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 38 + (i * (fontwidth * 1)), CR_ALIGN_LEFT, 7, cr_stage.stage_pointers[i]->name, false);
			}

			cr_spritefont_draw(&cr_devmenu.red, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 38 + (cr_devmenu.selection * (fontwidth * 1)), CR_ALIGN_LEFT, 7, cr_stage.stage_pointers[cr_devmenu.selection]->name, false);

			if (cr_input[CR_P1].START_PRESS || cr_input[CR_P1].A_PRESS) {
				crimson.skip_update = true;
				cr_clear_background();
				for (unsigned char i = 0; i < CR_DEFAULT_INPUT_COUNT; i++) {
					cr_input_clear(&cr_input[i]);
				}
				cr_stage_goto(cr_stage.stage_pointers[cr_devmenu.selection]);
				for (unsigned char i = 0; i < CR_DEFAULT_INPUT_COUNT; i++) {
					cr_input_clear(&cr_input[i]);
				}
				cr_devmenu.active = false;
				crimson.master_pause = false;
				cr_devmenu.selection = 0;
			}

			if (cr_input[CR_P1].B_PRESS) {
				cr_devmenu.mode = MODE_MAIN;
				cr_devmenu.selection = 0;
				cr_debug.step = true;
			}

			break;

		case MODE_GAMECONFIG:

			if (cr_input[CR_P1].UP_PRESS) {
				cr_devmenu.selection--;
				if (cr_devmenu.selection < 0) {
					cr_devmenu.selection = 16;
				}
			}
			if (cr_input[CR_P1].DOWN_PRESS) {
				cr_devmenu.selection++;
				if (cr_devmenu.selection > 16) {
					cr_devmenu.selection = 0;
				}
			}

			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 28, CR_ALIGN_LEFT, 7, "Gameconfig:", false);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 48 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Screen Mode:        < >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 48 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_windows.current_window_state);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 68 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Window Scale:       < >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 68 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.scale);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 88 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Borderless Window:  < >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 88 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_windows.borderless);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 108 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Scale Quality:      < >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 108 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_windows.scale_quality);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 128 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Master Volume:    <   >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 128 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 148 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Music Volume:     <   >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 148 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 168 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "SFX Volume:       <   >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 168 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 188 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Voice Volume:     <   >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 188 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 208 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Allow Devmenu:      < >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 208 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_debug.allow_devmenu);

			// SD Resolutions
			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 228 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Resolution X:  <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 228 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_width);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 238 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Resolution Y:  <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 238 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_height);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 248 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Width:         <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 248 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.width);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 258 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Height:        <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 258 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.height);

			// HD Resolutions
			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 278 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Resolution X:    <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 278 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_hd_width);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 288 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Resolution Y:    <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 288 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_hd_height);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 298 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Width:           <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 298 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.hd_width);

			cr_spritefont_draw(&cr_devmenu.white, fontwidth + cr_camera.cams[0]->pos.x, 308 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Height:          <    >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 308 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.hd_height);

			switch (cr_devmenu.selection)
			{
			case 0:
				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 48 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Screen Mode:        < >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 48 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_windows.current_window_state);
				if (cr_input[CR_P1].RIGHT_PRESS) {
					char state = cr_windows.current_window_state;
					if (state == 2) { state = 0; }
					else { state++; }
					cr_windows_set_window_state(state);
					cr_debug.step = true;
				}
				if (cr_input[CR_P1].LEFT_PRESS) {
					char state = cr_windows.current_window_state;
					if (state == 0) { state = 2; }
					else { state--; }
					cr_windows_set_window_state(state);
					cr_debug.step = true;
				}
				break;

			case 1:
				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 68 + +cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Window Scale:       < >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 68 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.scale);
				if (cr_input[CR_P1].RIGHT_PRESS) {
					crimson.scale++;
					cr_debug.step = true;
					if (crimson.scale > 8) {
						crimson.scale = 1;
					}
					SDL_SetWindowSize(cr_windows.window, crimson.game_width * crimson.scale, crimson.game_height * crimson.scale);
				}
				if (cr_input[CR_P1].LEFT_PRESS) {
					crimson.scale--;
					cr_debug.step = true;
					if (crimson.scale < 1) {
						crimson.scale = 8;
					}
					SDL_SetWindowSize(cr_windows.window, crimson.game_width * crimson.scale, crimson.game_height * crimson.scale);
				}
				break;

			case 2:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 88 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Borderless Window:  < >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 88 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_windows.borderless);

				if (cr_input[CR_P1].RIGHT_PRESS || cr_input[CR_P1].LEFT_PRESS) {
					SDL_SetWindowBordered(cr_windows.window, cr_windows.borderless);
					SDL_SetWindowPosition(cr_windows.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
					cr_windows.borderless = !cr_windows.borderless;
					cr_debug.step = true;
				}

				break;

			case 3:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 108 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Scale Quality:      < >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 108 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_windows.scale_quality);

				if (cr_input[CR_P1].RIGHT_PRESS || cr_input[CR_P1].LEFT_PRESS) {
					cr_windows.scale_quality = !cr_windows.scale_quality;
					cr_debug.step = true;
				}

				break;

			case 4:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 128 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Master Volume:    <   >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 128 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

				break;

			case 5:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 148 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Music Volume:     <   >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 148 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

				break;

			case 6:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 168 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "SFX Volume:       <   >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 168 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

				break;

			case 7:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 188 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Voice Volume:     <   >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 152) + cr_camera.cams[0]->pos.x, 188 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, 100);

				break;

			case 8:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 208 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Allow Devmenu:      < >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 208 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_debug.allow_devmenu);


				if (cr_input[CR_P1].RIGHT_PRESS || cr_input[CR_P1].LEFT_PRESS) {
					cr_debug.allow_devmenu = !cr_debug.allow_devmenu;
					cr_debug.step = true;
				}

				break;

			case 9:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 228 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Resolution X:  <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 228 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_width);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.game_width += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.game_width -= 16; }

				break;

			case 10:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 238 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Resolution Y:  <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 238 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_height);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.game_height += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.game_height -= 16; }

				break;

			case 11:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 248 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Width:         <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 248 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.width);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.width += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.width -= 16; }

				break;

			case 12:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 258 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Game Height:        <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 258 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.height);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.height += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.height -= 16; }

				break;

			case 13:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 278 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Resolution X:    <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 278 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_hd_width);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.game_hd_width += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.game_hd_width -= 16; }

				break;

			case 14:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 288 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Resolution Y:    <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 288 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.game_hd_height);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.game_hd_height += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.game_hd_height -= 16; }

				break;

			case 15:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 298 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Width:           <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 298 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.hd_width);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.hd_width += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.hd_width -= 16; }

				break;

			case 16:

				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 308 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "HD Height:          <    >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 308 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, crimson.hd_height);

				if (cr_input[CR_P1].RIGHT_PRESS) { cr_debug.step = true; crimson.hd_height += 16; }
				if (cr_input[CR_P1].LEFT_PRESS) { cr_debug.step = true; crimson.hd_height -= 16; }

				break;
			}

			if (cr_input[CR_P1].B_PRESS) {
				cr_devmenu.mode = MODE_MAIN;
				cr_devmenu.selection = 2;
				cr_debug.step = true;
			}

			break; // gameconfig

		case MODE_DEBUGOPTIONS:

			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 28, CR_ALIGN_LEFT, 7, "Debug Options:", false);

			cr_spritefont_draw(&cr_devmenu.white, cr_camera.cams[0]->pos.x + fontwidth, cr_camera.cams[0]->pos.y + 48, CR_ALIGN_LEFT, 7, "Fast Forward Speed: <  >", false);
			cr_counter_draw(&cr_devmenu.white_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 48 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_debug.fastforward_speed);

			switch (cr_devmenu.selection)
			{
			case 0:
				cr_spritefont_draw(&cr_devmenu.red, fontwidth + cr_camera.cams[0]->pos.x, 48 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, "Fast Forward Speed: <  >", false);
				cr_counter_draw(&cr_devmenu.red_nums, (fontwidth + 168) + cr_camera.cams[0]->pos.x, 48 + cr_camera.cams[0]->pos.y, CR_ALIGN_LEFT, 7, cr_debug.fastforward_speed);
				if (cr_input[CR_P1].RIGHT_PRESS) {
					cr_debug.fastforward_speed++;
					cr_debug.step = true;
					if (cr_debug.fastforward_speed > 69420) {
						cr_debug.fastforward_speed = 1;
					}
				}
				if (cr_input[CR_P1].LEFT_PRESS) {
					cr_debug.fastforward_speed--;
					cr_debug.step = true;
					if (cr_debug.fastforward_speed < 1) {
						cr_debug.fastforward_speed = 69420;
					}
				}
				break;
			}

			if (cr_input[CR_P1].B_PRESS) {
				cr_devmenu.mode = MODE_MAIN;
				cr_devmenu.selection = 1;
				cr_debug.step = true;
			}

			break; // debug options
		}

		if (cr_input[CR_P1].SELECT_PRESS) {
			cr_devmenu.active = false;
			crimson.master_pause = false;
		}
	}
	else {
		cr_devmenu.mode = 0;
		cr_devmenu.selection = 0;
	}
}
