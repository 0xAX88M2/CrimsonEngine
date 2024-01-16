#ifndef CR_WINDOWS_H
#define CR_WINDOWS_H

#if CR_PLATFORM == CR_WINDOWS

// ========================
// DEFINITIONS
// ========================

// Screens
#define CR_SCREEN_COUNT (4)

// ========================
// PLATFORM INCLUDES
// ========================

// C Standard Libraries
#define _CRTDBG_MAP_ALLOC  
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <crtdbg.h> 

// SDL Includes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer_ext.h>
#include <SDL_ttf.h>

// ================
// ENUMERATIONS
// ================

typedef enum {
	CR_WINDOWS_WINDOWED,
	CR_WINDOWS_MAXIMIZED,
	CR_WINDOWS_FULLSCREEN,
} CR_WINDOWS_WINDOW_STATE;

typedef enum {
	CR_WINDOWS_NEAREST,
	CR_WINDOWS_LINEAR,
} CR_WINDOWS_RENDER_QUALITY;

// ================
// STRUCTURES
// ================

//typedef struct {
//	SDL_Texture *screen;
//	bool active;
//	bool enabled;
//	bool drawn_on;
//} CR_WINDOWS_SCREEN;

typedef struct {

	// WINDOW INFO

	//unsigned int view_width;
	//unsigned int view_height;

	//unsigned int window_width;
	//unsigned int window_height;

	//float window_scale;
	unsigned char display_target;

	int window_displayindex;
	int window_id;

	bool is_fullscreen;
	bool borderless;

	bool hires_enabled;

	unsigned char scale_quality;

	// WINDOWS SPECIFIC

	unsigned char current_window_state;

	SDL_Texture *sd_screens[CR_SCREEN_COUNT];
	SDL_Texture *hd_screens[CR_SCREEN_COUNT];

	// SDL SPECIFIC

	SDL_Window		*window;		// Window object
	SDL_Renderer	*renderer;	// Effective "canvas"
	SDL_DisplayMode *display_info;	// Window Info
	SDL_Event		sdl_events;	// Do I actually need to comment this

} CR_WINDOWS_STRUCT;

CR_WINDOWS_STRUCT cr_windows;

int cr_windows_sdl_events(void);
void cr_windows_window_init(void);
void cr_windows_run(void);

void cr_windows_set_window_state(CR_WINDOWS_WINDOW_STATE window_state);

void cr_windows_layer_compile(void);
void cr_windows_clear(unsigned int clear_color);

#endif // !CR_PLATFORM == CR_WINDOWS

#endif // !CR_WINDOWS_H
