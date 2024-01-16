#ifndef __CR_DISPLAY_H__
#define __CR_DISPLAY_H__

//SDL
//Window objects
SDL_Window *cr_window;

//Effective "canvas"
SDL_Renderer *cr_renderer;

//Layers
#define CR_SD_LAYER_COUNT 8
SDL_Texture *cr_sd_layers[CR_SD_LAYER_COUNT];

#define CR_HD_LAYER_COUNT 2
SDL_Texture *cr_hd_layers[CR_HD_LAYER_COUNT];

#define CR_TOTAL_LAYER_COUNT (CR_SD_LAYER_COUNT + CR_HD_LAYER_COUNT)

unsigned char CR_LAYER_CURRENT_TARGET;

//END SDL

//----------------

//CRIMSON

//bool cr_screen.is_fullscreen;
//bool cr_screen.borderless_window;

//float cr_screen.window_scale;

//unsigned int cr_game.width;
//unsigned int cr_game.height;

//unsigned int cr_screen.view_width;
//unsigned int cr_screen.view_height;
//
//unsigned int cr_screen.window_width;
//unsigned int cr_screen.window_height;

typedef unsigned int CR_COLOR;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} CR_COLOR3;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} CR_COLOR4;

CR_COLOR4 cr_fill_color;

//unsigned char cr_screen.display_target;

typedef struct {
	SDL_DisplayMode info;
} CR_DISPLAY_INFO;

CR_DISPLAY_INFO *cr_display;

typedef struct {

	bool is_fullscreen;
	bool borderless_window;

	float window_scale;

	unsigned int view_width;
	unsigned int view_height;

	unsigned int window_width;
	unsigned int window_height;
	
	unsigned char display_target;

	int window_displayindex;
	int window_id;

	bool window_borderless;

	bool hires_enabled;

} CR_SCREEN_INFO;

CR_SCREEN_INFO cr_screen;

//int cr_screen.window_displayindex;
//int cr_screen.window_id;

typedef enum {
	CR_WINDOWED,
	CR_FULLSCREEN,
	CR_MAXIMIZED
} CR_WINDOW_STATE;

typedef enum {
	CR_NEAREST,
	CR_LINEAR
} CR_RENDER_QUALITY;

#define CR_DEFAULT_SCREEN_MODE (1)

CR_WINDOW_STATE cr_window_currentstate;

void cr_window_init(CR_WINDOW_STATE window_state, const char *window_name, unsigned int game_width, unsigned int game_height, unsigned int hd_width, unsigned int hd_height);

void cr_window_set_state(CR_WINDOW_STATE window_state);

//bool cr_screen.window_borderless;

//LAYERS
bool *cr_layer_lnscroll_enabled;

signed short **cr_layer_scroll_vals;

void cr_layer_set_target(unsigned char layer);

void cr_layer_compile(void);
void cr_layer_clear(const unsigned int clear_color);

void cr_layer_lnscroll_enable(unsigned char layer);
void cr_layer_lnscroll_disable(unsigned char layer);

void cr_display_set_target(unsigned char monitor);

#endif // !__CR_DISPLAY_H__
