#include "crimson.h"
#include "cr_win_input.h"

#if CR_PLATFORM == CR_WINDOWS

int cr_windows_sdl_events(void)
{
	int cont_count = 0;
	//Check for events
	while (SDL_PollEvent(&cr_windows.sdl_events)) {

		switch (cr_windows.sdl_events.window.event)
		{

		case SDL_CONTROLLERDEVICEADDED:
			cont_count = SDL_NumJoysticks();
			cr_windows_controller_init(cont_count - 1);
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			cr_windows_set_window_state(CR_WINDOWS_MAXIMIZED);
			cr_windows.is_fullscreen = true;
			break;
		}

		switch (cr_windows.sdl_events.type) {

		case SDL_WINDOWEVENT_CLOSE:

			if (cr_windows.window) {
				SDL_DestroyWindow(cr_windows.window);
				cr_windows.window = NULL;
			}

			return 1;

			break;

		case  SDL_KEYDOWN:

			switch (cr_windows.sdl_events.key.keysym.sym) {

			case SDLK_ESCAPE:
				if (cr_debug.allow_devmenu) {
#if CR_DEBUG
					cr_devmenu.active = !cr_devmenu.active;
					crimson.master_pause = false;
#endif
					if (!cr_devmenu.active) {
						cr_gameconfig_save();
						cr_clear_background();
					}
				}
				else {
					if (cr_windows.is_fullscreen) {
						cr_windows_set_window_state(CR_WINDOWS_WINDOWED);
						cr_windows.is_fullscreen = false;
					}
					else {
						cr_windows_set_window_state(CR_WINDOWS_MAXIMIZED);
						cr_windows.is_fullscreen = true;
					}
				}
				break;
			case SDLK_F3:
				cr_debug.game_debug = !cr_debug.game_debug;
				cr_debug.step = true;
				break;
			case SDLK_F4:
				if (cr_windows.is_fullscreen) {
					cr_windows_set_window_state(CR_WINDOWS_WINDOWED);
					cr_windows.is_fullscreen = false;
				}
				else {
					cr_windows_set_window_state(CR_WINDOWS_MAXIMIZED);
					cr_windows.is_fullscreen = true;
				}
				break;
			case SDLK_F5:
				cr_stage_goto(crimson.entry_stage);
				cr_devmenu.active = false;
				crimson.master_pause = false;
				break;
			case SDLK_F6:
				cr_stage_goto(cr_stage.current);
				cr_devmenu.active = false;
				crimson.master_pause = false;
				break;
			case SDLK_F7:
				cr_debug.step = true;
				break;
			case SDLK_F8:
				cr_debug.fastforward = true;
				break;
			}

			break;

		case SDL_KEYUP:
			case SDLK_F8:
				cr_debug.fastforward = false;
				break;
			break;

		case SDL_QUIT:
			return 1;
			break;
		}
	}

	return 0;
}

void cr_windows_window_init(void)
{

	// INIT SDL STUFF

	SDL_Init(SDL_INIT_EVERYTHING);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
#if CR_DEBUG
		printf("Crimson Debug: Failed to init SDL IMG: %s\n", IMG_GetError());
#endif
	}
	else {
#if CR_DEBUG
		printf("Crimson Debug: SDL IMG successfully initialized\n");
#endif
	}

	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
#if CR_DEBUG
		printf("Crimson Debug: Failed to init Audio Mixer. Error: %s\n", Mix_GetError());
#endif
	}
	else {
#if CR_DEBUG
		printf("Crimson Debug: Audio Mixer succesfully initialized\n");
#endif
	}

	if (TTF_Init() == -1) {
#if CR_DEBUG
		printf("Crimson Debug: Failed to init SDL TTF: %s\n", TTF_GetError());
#endif
	}
	else {
#if CR_DEBUG
		printf("Crimson Debug: TTF succesfully initialized\n");
#endif
	}

	// Set scale quality
	if (cr_windows.scale_quality) {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	}
	else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	}

	//Create window and renderer, set options
	cr_windows.window = SDL_CreateWindow(crimson.name, SDL_WINDOWPOS_CENTERED_DISPLAY(cr_windows.display_target), SDL_WINDOWPOS_UNDEFINED, crimson.game_width, crimson.game_height, SDL_WINDOW_RESIZABLE);
	cr_windows.renderer = SDL_CreateRenderer(cr_windows.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(cr_windows.renderer, SDL_BLENDMODE_NONE); //Allow for stacking of layers
	SDL_RenderSetLogicalSize(cr_windows.renderer, crimson.game_width, crimson.game_height);

	//Create Standard Def and Hi Def screens
	for (int i = 0; i < CR_SCREEN_COUNT; i++) {
		cr_windows.sd_screens[i] = SDL_CreateTexture(cr_windows.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, crimson.game_width, crimson.game_height);
		SDL_SetTextureBlendMode(cr_windows.sd_screens[i], SDL_BLENDMODE_BLEND);
	}

	for (int i = 0; i < CR_SCREEN_COUNT; i++) {
		cr_windows.hd_screens[i] = SDL_CreateTexture(cr_windows.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, crimson.hd_width, crimson.hd_height);
		SDL_SetTextureBlendMode(cr_windows.hd_screens[i], SDL_BLENDMODE_BLEND);
	}

	SDL_SetRenderTarget(cr_windows.renderer, cr_windows.sd_screens[cr_drawing.target_screen]);

	//Get monitor's displayindex...
	cr_windows.window_displayindex = SDL_GetWindowDisplayIndex(cr_windows.window);
	cr_windows.window_id = SDL_GetWindowID(cr_windows.window);

	//Size monitor struct to match number of monitors
	cr_windows.display_info = malloc(sizeof(SDL_DisplayMode) * SDL_GetNumVideoDisplays());

	// Get current display mode of all displays.
	for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
		int disp_mode = 0;
		if (cr_windows.display_info != NULL) {
			disp_mode = SDL_GetCurrentDisplayMode(i, &cr_windows.display_info[i]);
		}

#if CR_DEBUG
		//In case of error...
		if (disp_mode != 0) {
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
		}
#endif
	}

#if CR_DEBUG
	printf("Initial Window ID: %d\n", cr_windows.window_displayindex);
#endif

	cr_windows_set_window_state(cr_windows.current_window_state);
	SDL_SetWindowBordered(cr_windows.window, !cr_windows.borderless);
	SDL_SetWindowPosition(cr_windows.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

}

void cr_windows_run(void)
{
	cr_windows_window_init();
	
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		cr_windows_controller_init(i);
	}

	if (!crimson.gameconfig_loaded) {
		for (int i = 0; i < 4; i++) {
			cr_windows_input_map_default_keys(&cr_input[i], i);
			cr_windows_input_map_default_buttons(&cr_input[i]);
		}
	}

	// Pretend framecap initialization is here!

	crimson.running = true;

//#if CR_DEBUG
	cr_devmenu_load();
//#endif

	crimson.fr_init();
	cr_stage_goto(crimson.entry_stage);

	// Main Game Loop
	while (!crimson.exitcode) {

		crimson.exitcode = cr_windows_sdl_events();

		//printf("start of frame\n");

		// FRAME INIT
		crimson.framecount++;
		crimson.rngseed++;

		// FRAME CAP
		// (framecap code preferably goes here)

		// CLEAR FONTS
		cr_font_reset();

		// SET UP DRAWING

		crimson.draw_calls = 0;

		if (!cr_devmenu.active) {
			if (!crimson.master_pause) {
				if (crimson.wipe_background) {
					cr_font_reset();
					cr_clear_background();
				}
			}
		}
		cr_internal_queue_clear();

		// RUN INPUT
		for (int i = 0; i < 4; i++) {
			cr_windows_input_update(&cr_input[i]);
		}

		if (!cr_devmenu.active) {
			if (!cr_debug.fastforward) {
				if (!crimson.master_pause) {
					cr_font_reset();
					crimson.fr_update_start();
					if (cr_stage.current != NULL) { cr_stage.current->update(); }
					crimson.fr_update_end();
					cr_camera_update();
				}
			}
		}
		else {
			if (crimson.master_pause) {
				if (cr_debug.step) {
					cr_font_reset();
					cr_clear_background();
					cr_internal_queue_clear();
					crimson.fr_update_start();
					if (cr_stage.current != NULL) { cr_stage.current->update(); }
					crimson.fr_update_end();
					cr_camera_update();
					cr_debug.step = false;
				}
			}
		}

		// fastforward
		if (cr_debug.fastforward) {
			for (int i = 0; i < cr_debug.fastforward_speed; i++) {
				if (i < cr_debug.fastforward_speed) {
					cr_font_reset();
					cr_internal_queue_clear();
				}
				crimson.fr_update_start();
				if (cr_stage.current != NULL) { cr_stage.current->update(); }
				crimson.fr_update_end();
				cr_camera_update();
				cr_debug.step = false;
			}
		}

		cr_devmenu_run();

		cr_windows_layer_compile();

		//if (cr_devmenu.active) { SDL_RenderPresent(cr_windows.prev_renderer); }
		SDL_RenderPresent(cr_windows.renderer);

		//crimson.exitcode = cr_windows_sdl_events();
	}

	// ================
	// CLOSING DOWN
	// ================

#if CR_DEBUG
	printf("Exiting game loop\n");
#endif

	// Crimson
	cr_devmenu_unload();
	crimson.fr_close();
	crimson_close();
	cr_internal_queue_clear();

	// Windows
	SDL_DestroyWindow(cr_windows.window);
	SDL_DestroyRenderer(cr_windows.renderer);
	free(cr_windows.display_info);

	// SDL
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#if CR_DEBUG
	printf("Was there a leak? %d\n", _CrtDumpMemoryLeaks());
#endif

	//system("pause");
}

void cr_windows_set_window_state(CR_WINDOWS_WINDOW_STATE window_state)
{
	if (crimson.running) {
		if (cr_windows.current_window_state == window_state) {
			return;
		}
	}

	cr_windows.current_window_state = window_state;

	switch (window_state) {

	case CR_WINDOWS_WINDOWED:

		SDL_SetWindowFullscreen(cr_windows.window, 0);
		SDL_SetWindowSize(cr_windows.window, (int)(crimson.game_width * (float)crimson.scale), (int)(crimson.game_height * (float)crimson.scale));
		SDL_RestoreWindow(cr_windows.window);
		SDL_SetWindowPosition(cr_windows.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		cr_windows.is_fullscreen = false;
		
		break;

	case CR_WINDOWS_MAXIMIZED:

		SDL_SetWindowFullscreen(cr_windows.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_RestoreWindow(cr_windows.window);
		cr_windows.is_fullscreen = true;

		break;

	case CR_WINDOWS_FULLSCREEN:

		SDL_SetWindowSize(cr_windows.window, cr_windows.display_info[cr_windows.display_target].w, cr_windows.display_info[cr_windows.display_target].h);
		SDL_SetWindowFullscreen(cr_windows.window, SDL_WINDOW_FULLSCREEN);
		cr_windows.is_fullscreen = true;
		
		break;
	
	}

	if (!crimson.running) {
		SDL_SetWindowPosition(cr_windows.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

void cr_windows_layer_compile(void)
{
	SDL_Rect dest_sd[CR_SCREEN_COUNT];
	SDL_Rect dest_hd[CR_SCREEN_COUNT];

	switch (cr_drawing.active_screens)
	{
	case 0:

		dest_sd[0].x = 0;
		dest_sd[0].y = 0;
		dest_sd[0].w = crimson.game_width;
		dest_sd[0].h = crimson.game_height;

		break;

	case 1: 

		if (cr_drawing.horiz_splitscreen) {
			dest_sd[0].x = (crimson.game_width / 4);
			dest_sd[0].y = 0;
			dest_sd[0].w = crimson.game_width / 2;
			dest_sd[0].h = crimson.game_height / 2;

			dest_sd[1].x = (crimson.game_width / 4);
			dest_sd[1].y = (crimson.game_height / 2);
			dest_sd[1].w = crimson.game_width / 2;
			dest_sd[1].h = crimson.game_height / 2;
		}
		else {
			dest_sd[0].x = 0;
			dest_sd[0].y = (crimson.game_height / 4);
			dest_sd[0].w = crimson.game_width / 2;
			dest_sd[0].h = crimson.game_height / 2;

			dest_sd[1].x = (crimson.game_width / 2);
			dest_sd[1].y = (crimson.game_height / 4);
			dest_sd[1].w = crimson.game_width / 2;
			dest_sd[1].h = crimson.game_height / 2;
		}

		break;

	case 2:

		dest_sd[0].x = 0;
		dest_sd[0].y = 0;
		dest_sd[0].w = crimson.game_width / 2;
		dest_sd[0].h = crimson.game_height / 2;

		dest_sd[1].x = (crimson.game_width / 2);
		dest_sd[1].y = 0;
		dest_sd[1].w = crimson.game_width / 2;
		dest_sd[1].h = crimson.game_height / 2;

		dest_sd[2].x = (crimson.game_width / 4);
		dest_sd[2].y = (crimson.game_height / 2);
		dest_sd[2].w = crimson.game_width / 2;
		dest_sd[2].h = crimson.game_height / 2;

		break;

	case 3:
		
		dest_sd[0].x = 0;
		dest_sd[0].y = 0;
		dest_sd[0].w = crimson.game_width / 2;
		dest_sd[0].h = crimson.game_height / 2;

		dest_sd[1].x = (crimson.game_width / 2);
		dest_sd[1].y = 0;
		dest_sd[1].w = crimson.game_width / 2;
		dest_sd[1].h = crimson.game_height / 2;

		dest_sd[2].x = 0;
		dest_sd[2].y = (crimson.game_height / 2);
		dest_sd[2].w = crimson.game_width / 2;
		dest_sd[2].h = crimson.game_height / 2;

		dest_sd[3].x = (crimson.game_width / 2);
		dest_sd[3].y = (crimson.game_height / 2);
		dest_sd[3].w = crimson.game_width / 2;
		dest_sd[3].h = crimson.game_height / 2;

		break;
	}

	dest_hd[0].x = 0;
	dest_hd[0].y = 0;
	dest_hd[0].w = dest_sd[0].w;
	dest_hd[0].h = dest_sd[0].h;

	for (unsigned char i = 0; i <= cr_drawing.active_screens; i++) {
		cr_drawing.target_screen = i;
		cr_internal_queue_draw();
		SDL_SetRenderTarget(cr_windows.renderer, NULL);
		SDL_RenderCopy(cr_windows.renderer, cr_windows.sd_screens[i], NULL, &dest_sd[i]);
		SDL_RenderCopy(cr_windows.renderer, cr_windows.hd_screens[i], NULL, &dest_sd[i]);
		SDL_RenderSetClipRect(cr_windows.renderer, NULL);
	}
}

void cr_windows_clear(unsigned int clear_color)
{
	//Get clear color
	unsigned char r = (clear_color >> (8 * 2)) & 0x0000ff;
	unsigned char g = (clear_color >> (8 * 1)) & 0x0000ff;
	unsigned char b = (clear_color >> (8 * 0)) & 0x0000ff;

	//Clear main renderer
	SDL_SetRenderTarget(cr_windows.renderer, NULL);
	SDL_SetRenderDrawColor(cr_windows.renderer, r, g, b, 0);
	SDL_RenderClear(cr_windows.renderer);

	for (unsigned char i = 0; i < CR_SCREEN_COUNT; i++) {
		SDL_SetRenderTarget(cr_windows.renderer, cr_windows.sd_screens[i]);
		SDL_SetRenderDrawColor(cr_windows.renderer, r, g, b, 0);
		SDL_RenderClear(cr_windows.renderer);
	}
	for (unsigned char i = 0; i < CR_SCREEN_COUNT; i++) {
		SDL_SetRenderTarget(cr_windows.renderer, cr_windows.hd_screens[i]);
		SDL_SetRenderDrawColor(cr_windows.renderer, r, g, b, 0);
		SDL_RenderClear(cr_windows.renderer);
	}
}

#endif // !CR_PLATFORM == CR_WINDOWS
