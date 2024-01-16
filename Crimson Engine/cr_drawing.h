#ifndef CR_DRAWING_H
#define CR_DRAWING_H

typedef enum {
	CR_DRAWTYPE_STANDARD,
	CR_DRAWTYPE_PARALLAX,
	CR_DRAWTYPE_STAGE,
} CR_DRAW_TYPES;

typedef enum {
	CR_LNSCROLL_HORIZ,
	CR_LNSCROLL_VERTI,
} CR_LNSCROLL_MODE;

typedef enum {
	CR_RGB_BLACK		= 0x000000,
	CR_RGB_WHITE		= 0xffffff,
	CR_RGB_GREY			= 0x808080,
	CR_RGB_GRAY			= 0x808080,
	CR_RGB_GREEN		= 0x00ff00,
	CR_RGB_MAGENTA		= 0xff00ff,
	CR_RGB_YELLOW		= 0xffff00,
	CR_RBG_NICER_YELLOW	= 0xfff010,
	CR_RGB_RED			= 0xff0000,
	CR_RGB_BLUE			= 0x0000ff,
	CR_RGB_PURPLE		= 0x800080,
	CR_RGB_CYAN			= 0x00ffff,
	CR_RGB_ORANGE		= 0xff8000,
} CR_RGB_ENUM;

typedef enum {
	CR_RGBA_BLACK			= 0x000000ff,
	CR_RGBA_WHITE			= 0xffffffff,
	CR_RGBA_GREY			= 0x808080ff,
	CR_RGBA_GRAY			= 0x808080ff,
	CR_RGBA_GREEN			= 0x00ff00ff,
	CR_RGBA_BLANK			= 0x00000000,
	CR_RGBA_MAGENTA			= 0xff00ffff,
	CR_RGBA_YELLOW			= 0xffff00ff,
	CR_RBGA_NICER_YELLOW	= 0xfff010ff,
	CR_RGBA_RED				= 0xff0000ff,
	CR_RGBA_BLUE			= 0x0000ffff,
	CR_RGBA_PURPLE			= 0x800080ff,
	CR_RGBA_CYAN			= 0x00ffffff,
	CR_RGBA_ORANGE			= 0xff8000ff,
} CR_RGBA_ENUM;

typedef struct
{
	// display settings
	CR_COLOR clear_color;
	bool hires_enabled;
	unsigned target_screen;

	//blending
	bool blending_enabled;
	unsigned char blend_amount;
	unsigned char blend_mode;
	bool clip_mode;

	//parallax
	bool false_parallax;
	bool bgcam_mode;

	//splitscreen
	unsigned char active_screens;
	bool horiz_splitscreen;
	unsigned char screen_delegation;

} CR_DRAWING_STRUCT;

CR_DRAWING_STRUCT cr_drawing;


void cr_internal_queue_add(CR_TEXTURE *sprite_id, float pos_x, float pos_y, char disp_x, char disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char draw_type, double rate_x, double rate_y, unsigned char par_type, unsigned char flip_flag, unsigned char draw_layer);
void cr_internal_queue_draw(void);
void cr_internal_queue_clear(void);

void cr_internal_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short disp_x, short disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//Drawing static, non-animated, single frame graphics
void cr_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_sprite_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_sprite_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_sprite_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_sprite_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//You can use these to draw a section of a bitmap
void cr_section_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, unsigned char flip_flag, unsigned char draw_layer);
void cr_section_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_section_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_section_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_section_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//Draw and play an animation
void cr_anim_draw(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_draw_lnscroll(CR_ANIM* anim_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_draw_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_draw_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_draw_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//Display but not play an animation
void cr_anim_display(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_display_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_display_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_display_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//Display a specific animation frame
void cr_anim_display_frame(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_display_frame_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_display_frame_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_anim_display_frame_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);

void cr_script_draw(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_draw_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_draw_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_draw_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

void cr_script_display(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_display_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_display_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_display_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

void cr_script_display_frame(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_display_frame_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_display_frame_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_script_display_frame_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer);

//PARALLAX FUNCTIONS
void cr_parallax_draw(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer);
void cr_parallax_draw_lnscroll(CR_TEXTURE* sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_parallax_draw_animated(CR_ANIM *anim_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer);
void cr_parallax_draw_section(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, int spr_x, int spr_y, int spr_width, int spr_height, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer);

// OTHER FUNCTIONS
void cr_hires_toggle(bool enable);
void cr_screen_delegate(unsigned char screen_id);
void cr_screen_undelegate(void);

#endif // !CR_DRAWING_H
