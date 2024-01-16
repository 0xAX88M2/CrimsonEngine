#ifndef CR_SPRITES_H
#define CR_SPRITES_H

//DEFINITONS
#define CR_TILE_COUNT(TILES)    (sizeof(TILES) / sizeof(CR_TILE))

#define CR_DEFAULT_LAYER_COUNT	(8)
#define CR_DEFAULT_LAYER_LIMIT	(8192)
signed int cr_draw_slot_count[CR_DEFAULT_LAYER_COUNT];

//TYPES
typedef unsigned int CR_COLOR;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} CR_COLOR3;

typedef struct {
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
} CR_COLOR4;

short cr_lnscroll[1024];

#if CR_PLATFORM == CR_DIET

#endif // CR_PLATFORM == CR_DIET

#if CR_PLATFORM == CR_WINDOWS
typedef SDL_Texture *CR_TEXTURE;
#endif // CR_PLATFORM == CR_WINDOWS

#if CR_PLATFORM == CR_LINUX

#endif // CR_PLATFORM == CR_LINUX

#if CR_PLATFORM == CR_ANDROID

#endif // CR_PLATFORM == CR_ANDROID

typedef enum {
	CR_ALIGN_LEFT,
	CR_ALIGN_RIGHT,
	CR_ALIGN_CENTER,
} CR_ALIGNMENT;

typedef enum {
	CR_ANIM_LOOP,
	CR_ANIM_REVERSE,
	CR_ANIM_LOOP_ONCE,
	CR_ANIM_REVERSE_ONCE,
	CR_ANIM_PINGPONG,
} CR_ANIM_TYPE;

typedef enum {
	CR_NO_FLIP,
	CR_FLIP_H,
	CR_FLIP_V,
} CR_SPRITE_FLIP;

typedef enum {
	CR_BG_NO = 0x00,		// No BG looping
	CR_BG_H = 0x01,			// Horizontal BG looping
	CR_BG_V = 0x02,			// Vertical BG looping
	CR_BG_ONEWAY_R = 0x04,	// 
	CR_BG_ONEWAY_L = 0x08,	//
	CR_BG_LNSCROLL_HORIZ = 0x10,
	CR_BG_LNSCROLL_VERTI = 0x20,
} CR_PARALLAX_TYPE;

//Blending
typedef enum {
	CR_BLEND_NONE,
	CR_BLEND_ALPHA,
	CR_BLEND_ADD,
	CR_BLEND_MOD = 0x4,
} CR_BLEND;

typedef enum {
	CR_DRAW,
	CR_DRAW_SCALED,
	CR_DRAW_ROTATED,
	CR_DRAW_SCALED_ROTATED,
} CR_DRAW_TYPE;

//Tile struct
typedef struct
{
	unsigned short	x;
	unsigned short	y;
	unsigned char	width;
	unsigned char	height;
	signed char		disp_x;
	signed char		disp_y;
} CR_TILE;

//Anim struct
typedef struct
{
	CR_TEXTURE	*sprite;					// Sprite sheet to use
	CR_TEXTURE **spritelist;
	unsigned char *path;					// Path of the current sprite sheet
	CR_TILE		*tileset;					// Animation tileset to use

	unsigned short id;						// ID of the current animation script (if any)
	unsigned short prev_id;					// Previous ID (if any)

	unsigned char speed;					// How fast to play the animation
	unsigned short frame_count;				// Amount of frames in the animation script
	short frame_current;					// Current frame to display
	short frame_previous;					// Current frame to display
	unsigned short frame_timer;				// Time until the next frame
	unsigned short loop_frame;				// Frame number to loop back to

	unsigned short spr_x;					// Origin X of the current frame on the sheet
	unsigned short spr_y;					// Origin Y of the current frame on the sheet
	unsigned short width;					// Width of the current frame
	unsigned short height;					// Height of the current frame
	signed short disp_x;					// Horizontal displacement value for display
	signed short disp_y;					// Vertical displacement value for display

	unsigned char anim_type;				// Flag for the type of animation

	bool finished;							// Flag for when the animation finishes
	bool changed;							// Flag for when the animation is changed
	bool initialized;						// Flag for if the animation is initialized
	bool sequence;							// Flag for if the animation is a sequence
	bool global;							// Flag for if the animation is a sequence and globally-loaded

} CR_ANIM;

typedef struct {
	CR_TILE *tileset;
	unsigned char sheet_id;
	unsigned int framecount;
	unsigned char speed;
	CR_ANIM_TYPE type;
	unsigned char loop_frame;
} CR_ANIM_SCRIPT;

typedef struct {
	unsigned short id;

	unsigned char sheet_cnt;
	CR_TEXTURE **sheets;

	unsigned short count;
	CR_ANIM_SCRIPT **scripts;

	bool initialized;
} CR_ANIM_DATA;

//DRAW DATA
typedef struct {

	CR_TEXTURE *sprite_data;

	float pos_x;
	float pos_y;

	short disp_x;
	short disp_y;

	short spr_x;
	short spr_y;

	short spr_width;
	short spr_height;

	short origin_x;
	short origin_y;

	float scale_x;
	float scale_y;

	float rot_degree;

	unsigned char flip_flag;
	unsigned char draw_layer;
	unsigned char screen_delegation;

	bool blending;
	unsigned char blend_mode;
	unsigned char blend_amount;

	bool is_hd;

	// Parallax
	unsigned char draw_type;
	double rate_x;
	double rate_y;
	unsigned char par_type;
	bool bgcam;

} CR_QUEUE_DATA;

typedef struct {

	unsigned short sequence_count;
	CR_ANIM **sequence_pointers;

	unsigned short global_sequence_count;
	CR_ANIM **global_sequence_pointers;

	bool initialized;

} CR_SPRITE_MANAGER;

CR_SPRITE_MANAGER cr_sprites;

//LOADING FUNCTIONS
CR_TEXTURE* cr_sprite_load(const char *filepath, int transparent_color);
void cr_sprite_unload(CR_TEXTURE *sprite_id);

void cr_sprite_manager_reset(bool unload_globals);

//ANIMATION FUNCTIONS

void cr_script_load(const char *filepath, CR_ANIM_DATA *anim_data, CR_ANIM *anim, unsigned short start_id);
void cr_script_unload(CR_ANIM_DATA *anim_data);

//Animation management
void cr_anim_add(CR_TEXTURE *sprite, CR_ANIM *anim_id, CR_TILE *tileset, unsigned short framecount, unsigned char speed, unsigned char type);
void cr_anim_sequence_add(CR_ANIM *anim_id, CR_ANIM_TYPE anim_type, const unsigned short framecount);
void cr_anim_sequence_remove(CR_ANIM *anim_id);
void cr_anim_reset(CR_ANIM *anim_id);
void cr_anim_unload(CR_ANIM *anim_id);
void cr_script_add(CR_ANIM_DATA *anim_data, CR_ANIM *anim_id, unsigned short anim_num);
void cr_script_add_sheet(CR_ANIM_DATA *anim_data, CR_ANIM *anim_id, unsigned short anim_num, unsigned short sheet_num);
void cr_anim_transfer(CR_ANIM *target_anim, CR_ANIM *source_anim);

//Change the way an animation plays
void cr_anim_set_type(CR_ANIM *anim_id, CR_ANIM_TYPE animtype);

//Get/Set post frame
unsigned short cr_anim_get_frame(CR_ANIM *anim_id);
void cr_anim_set_frame(CR_ANIM *anim_id, unsigned short frame);

//Get/Set post speed
unsigned short cr_anim_get_speed(CR_ANIM *anim_id);
void cr_anim_set_speed(CR_ANIM *anim_id, unsigned char anim_speed);

//Get/Set loop point
unsigned short cr_anim_get_loop_frame(CR_ANIM *anim_id);
void cr_anim_set_loop_frame(CR_ANIM *anim_id, unsigned short loopframe);

//Play an animation without drawing
void cr_anim_play(CR_ANIM *anim_id);

//Count tiles in tile struct
unsigned short cr_count_tiles(const CR_TILE *tiles);

//BLEND FUNCTIONS
void cr_blending_enable(unsigned char blend_mode, unsigned char blend_amount);
void cr_blending_disable(void);

// Sprite write
void cr_counter_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, int draw_number);
void cr_spritefont_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, const char *const draw_string, bool follow_camera);
unsigned int cr_get_letter_number(char product);

void cr_lnscroll_wave(short degree, short root, short osc_mag, float osc_width, float osc_freq);

#endif // !CR_SPRITES_H
