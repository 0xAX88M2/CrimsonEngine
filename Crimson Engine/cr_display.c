#include "crimson.h"

void cr_window_init(CR_WINDOW_STATE fullscreen_mode, const char *window_name, unsigned int game_width, unsigned int game_height, unsigned int hd_width, unsigned int hd_height)
{
	//SET RESOLUTIONS
	cr_game.width = game_width;
	cr_game.height = game_height;

	cr_screen.window_width = game_width;
	cr_screen.window_height = game_height;

	cr_screen.view_width = hd_width;
	cr_screen.view_height = hd_height;

	CR_CLEAR_COLOR = 0x000000;

	//SDL STUFF
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_AUDIO);
	(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);

	// Debug Logging

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
#if CRIMSON_DEBUG == 1
		printf("Failed to init SDL IMG: %s\n", IMG_GetError());
#endif
	}
	else {
#if CRIMSON_DEBUG == 1
		printf("SDL IMG for PNG successfully initialized\n");
#endif
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
#if CRIMSON_DEBUG == 1
		printf("Failed to init Audio Mixer. Error: %d\n", Mix_GetError());
#endif
	}
	else {
#if CRIMSON_DEBUG == 1
		printf("Audio Mixer succesfully initialized\n", Mix_GetError());
#endif
	}

	//Create window and renderer, set options
	cr_window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED_DISPLAY(cr_screen.display_target), SDL_WINDOWPOS_UNDEFINED, game_width, game_height, SDL_WINDOW_RESIZABLE);
	//cr_renderer = SDL_CreateRenderer(cr_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	cr_renderer = SDL_CreateRenderer(cr_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(cr_renderer, SDL_BLENDMODE_BLEND); //Allow for stacking of layers

	SDL_RenderSetLogicalSize(cr_renderer, game_width, game_height);
	//Create Standard Def and Hi Def layers
	for (int i = 0; i < (CR_SD_LAYER_COUNT); i++) {
		cr_sd_layers[i] = SDL_CreateTexture(cr_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, cr_game.width, cr_game.height);
		SDL_SetTextureBlendMode(cr_sd_layers[i], SDL_BLENDMODE_BLEND);
	}

	for (int i = 0; i < (CR_HD_LAYER_COUNT); i++) {
		cr_hd_layers[i] = SDL_CreateTexture(cr_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, cr_screen.view_width, cr_screen.view_height);
		SDL_SetTextureBlendMode(cr_hd_layers[i], SDL_BLENDMODE_BLEND);
	}

	//LAYER LINESCROLL VARS
	cr_layer_lnscroll_enabled = cr_calloc(CR_TOTAL_LAYER_COUNT, sizeof(bool));


	//Get monitor's displayindex...
	cr_screen.window_displayindex = SDL_GetWindowDisplayIndex(cr_window);
	cr_screen.window_id = SDL_GetWindowID(cr_window);
#if CRIMSON_DEBUG == 1
	printf("Starting Monitor: %d\n", cr_screen.window_displayindex);
	printf("Window ID: %d\n", cr_screen.window_displayindex);
#endif

	//Size monitor struct to match number of monitors
	cr_display = malloc(sizeof(CR_DISPLAY_INFO) * SDL_GetNumVideoDisplays());

	// Get current display mode of all displays.
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
		int disp_mode = SDL_GetCurrentDisplayMode(i, &cr_display[i].info);


		//In case of error...
		if (disp_mode != 0) {
#if CRIMSON_DEBUG == 1
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
#endif
		}
		else { //On success, print the current display mode.
#if CRIMSON_DEBUG == 1
			printf("Display %d: %dx%dpx @ %dhz\n", i, cr_display[i].info.w, cr_display[i].info.h, cr_display[i].info.refresh_rate);
#endif
		}
	}

	cr_layer_scroll_vals = malloc(CR_TOTAL_LAYER_COUNT * sizeof(signed short*));
	for (int i = 0; i < CR_TOTAL_LAYER_COUNT; i++) {
		cr_layer_scroll_vals[i] = calloc(cr_game.height, 2);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	cr_window_set_state(fullscreen_mode);
}

void cr_window_set_state(CR_WINDOW_STATE window_state)
{
	if (cr_window_currentstate == window_state)
		return;

	cr_window_currentstate = window_state;
	switch (window_state) {
	case CR_WINDOWED:
		SDL_SetWindowFullscreen(cr_window, 0);
		SDL_SetWindowSize(cr_window, cr_game.width * cr_screen.window_scale, cr_game.height * cr_screen.window_scale);
		SDL_RestoreWindow(cr_window);

		cr_screen.is_fullscreen = false;
		break;

	case CR_FULLSCREEN:
		SDL_SetWindowSize(cr_window, cr_display[cr_screen.display_target].info.w, cr_display[cr_screen.display_target].info.h);
		SDL_SetWindowFullscreen(cr_window, SDL_WINDOW_FULLSCREEN);
		cr_screen.is_fullscreen = true;
		break;

	case CR_MAXIMIZED:
		SDL_RestoreWindow(cr_window);
		SDL_SetWindowFullscreen(cr_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		cr_screen.is_fullscreen = true;
		break;
	}
}

void cr_layer_set_target(unsigned char layer)
{
	if (layer > 0 && layer < CR_TOTAL_LAYER_COUNT) {
		SDL_SetRenderTarget(cr_renderer, cr_sd_layers[layer - 1]);
	}
	else {
		//SDL_SetRenderTarget(cr_renderer, cr_hd_layers[1]);
	}

	if (layer == 0) {
		//DL_SetRenderTarget(cr_renderer, cr_hd_layers[0]);
	}

	CR_LAYER_CURRENT_TARGET = layer;
}

void cr_display_set_target(unsigned char monitor)
{

}

void cr_layer_compile(void)
{
	SDL_SetRenderTarget(cr_renderer, NULL);

	//SDL_RenderCopy(cr_renderer, cr_hd_layers[0], NULL, NULL);

	for (int i = 0; i < CR_SD_LAYER_COUNT; i++) {
		//SDL_RenderCopy(cr_renderer, cr_sd_layers[i], NULL, NULL);
	}
	SDL_RenderCopy(cr_renderer, cr_sd_layers[0], NULL, NULL);
	//SDL_RenderCopy(cr_renderer, cr_sd_layers[3], NULL, NULL);

	//SDL_RenderCopy(cr_renderer, cr_hd_layers[1], NULL, NULL);
}

void cr_layer_clear(const unsigned int clear_color)
{
	//Get clear color
	unsigned char r = (clear_color >> (8 * 2)) & 0x0000ff;
	unsigned char g = (clear_color >> (8 * 1)) & 0x0000ff;
	unsigned char b = (clear_color >> (8 * 0)) & 0x0000ff;

	//Clear main renderer
	SDL_SetRenderTarget(cr_renderer, NULL);
	//SDL_SetRenderDrawColor(cr_renderer, r, g, b, 0);
	//SDL_RenderClear(cr_renderer);

	//Clear layers
	//SDL_SetRenderTarget(cr_renderer, cr_hd_layers[0]);
	//SDL_SetRenderDrawColor(cr_renderer, r, g, b, 0);
	//SDL_RenderClear(cr_renderer);
	for (int i = 0; i < CR_SD_LAYER_COUNT; i++) {
		//SDL_SetRenderTarget(cr_renderer, cr_sd_layers[i]);
		//SDL_SetRenderDrawColor(cr_renderer, r, g, b, 0);
		//SDL_RenderClear(cr_renderer);
	}
	SDL_SetRenderTarget(cr_renderer, cr_sd_layers[0]);
	SDL_SetRenderDrawColor(cr_renderer, r, g, b, 0);
	SDL_RenderClear(cr_renderer);
	//SDL_SetRenderTarget(cr_renderer, cr_hd_layers[1]);
	//SDL_SetRenderDrawColor(cr_renderer, r, g, b, 0);
	//SDL_RenderClear(cr_renderer);
}

void cr_layer_lnscroll_enable(unsigned char layer)
{
	//cr_layer_lnscroll_enabled[layer] = true;

	for (int i = 0; i < cr_game.height; i++) {
		//cr_layer_scroll_vals[layer][i] = 0;
	}
}

void cr_layer_lnscroll_disable(unsigned char layer)
{
	cr_layer_lnscroll_enabled[layer] = false;
}


