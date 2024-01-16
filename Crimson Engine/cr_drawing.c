#include "crimson.h"

#if CR_PLATFORM == CR_WINDOWS
#include "cr_win_drawing.h"
#endif // CR_PLATFORM == CR_WINDOWS

void cr_internal_queue_add(CR_TEXTURE *sprite_id, float pos_x, float pos_y, char disp_x, char disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char draw_type, double rate_x, double rate_y, unsigned char par_type, unsigned char flip_flag, unsigned char draw_layer)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, scale_x, scale_y, rot_degree, draw_type, rate_x, rate_y, par_type, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_internal_queue_draw(void)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_internal_queue_draw();
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_internal_queue_clear(void)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_internal_queue_clear();
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_internal_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short disp_x, short disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_internal_sprite_draw(sprite_id, pos_x, pos_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer, false);
#endif // CR_PLATFORM == CR_WINDOWS

}

//Drawing static, non-animated, single frame graphics
void cr_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sprite_draw(sprite_id, pos_x, pos_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_sprite_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sprite_draw_lnscroll(sprite_id, pos_x, pos_y, lnscroll_vals, lnscroll_mode, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_sprite_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sprite_draw_scaled(sprite_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_sprite_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sprite_draw_rotated(sprite_id, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_sprite_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sprite_draw_scaled_rotated(sprite_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

//You can use these to draw a section of a bitmap
void cr_section_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_section_draw(sprite_id, pos_x, pos_y, spr_x, spr_y, spr_width, spr_height, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_section_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_section_draw_lnscroll(sprite_id, pos_x, pos_y, spr_x, spr_y, spr_width, spr_height, lnscroll_vals, lnscroll_mode, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_section_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_section_draw_scaled(sprite_id, pos_x, pos_y, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_section_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_section_draw_rotated(sprite_id, pos_x, pos_y, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_section_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_section_draw_scaled(sprite_id, pos_x, pos_y, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

//Draw and play an animation
void cr_anim_draw(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_draw(anim_id, pos_x, pos_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_draw_lnscroll(CR_ANIM* anim_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_draw_lnscroll(anim_id, pos_x, pos_y, lnscroll_vals, lnscroll_mode, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_draw_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_draw_scaled(anim_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_draw_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_draw_rotated(anim_id, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_draw_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_draw_scaled_rotated(anim_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

//Display but not play an animation
void cr_anim_display(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display(anim_id, pos_x, pos_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_display_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display_scaled(anim_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_display_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display_rotated(anim_id, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_display_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display_scaled_rotated(anim_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

//Display a specific animation frame
void cr_anim_display_frame(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display_frame(anim_id, pos_x, pos_y, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_display_frame_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display_frame_scaled(anim_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_display_frame_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display_frame_rotated(anim_id, pos_x, pos_y, origin_x, origin_y, rot_degree, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_anim_display_frame_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_anim_display_frame_scaled_rotated(anim_id, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_draw(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer) 
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_draw(anim_data, anim, pos_x, pos_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_draw_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_draw_scaled(anim_data, anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_draw_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_draw_rotated(anim_data, anim, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_draw_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_draw_scaled_rotated(anim_data, anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_display(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display(anim_data, anim, pos_x, pos_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_display_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display_scaled(anim_data, anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_display_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display_rotated(anim_data, anim, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_display_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display_scaled_rotated(anim_data, anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}


void cr_script_display_frame(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display_frame(anim_data, anim, pos_x, pos_y, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_display_frame_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display_frame_scaled(anim_data, anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_display_frame_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display_frame_rotated(anim_data, anim, pos_x, pos_y, origin_x, origin_y, rot_degree, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_script_display_frame_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char display_frame, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_script_display_frame_scaled_rotated(anim_data, anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, display_frame, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

//PARALLAX FUNCTIONS
void cr_parallax_draw(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer)
{
	
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_parallax_draw(sprite_id, offset_x, offset_y, rate_x, rate_y, type_flag, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_parallax_draw_lnscroll(CR_TEXTURE* sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_parallax_draw_lnscroll(sprite_id, offset_x, offset_y, rate_x, rate_y, type_flag, lnscroll_vals, lnscroll_mode, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_parallax_draw_animated(CR_ANIM *anim_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_parallax_draw_animated(anim_id, offset_x, offset_y, rate_x, rate_y, type_flag, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_parallax_draw_section(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, int spr_x, int spr_y, int spr_width, int spr_height, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer)
{
	
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_parallax_draw_section(sprite_id, offset_x, offset_y, rate_x, rate_y, spr_x, spr_y, spr_width, spr_height, type_flag, flip_flag, draw_layer);
#endif // CR_PLATFORM == CR_WINDOWS

}

//HI RES
void cr_hires_toggle(bool enable)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_hires_toggle(enable);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_screen_delegate(unsigned char screen_id)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_screen_delegate(screen_id);
#endif // CR_PLATFORM == CR_WINDOWS

}

void cr_screen_undelegate(void)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_screen_undelegate();
#endif // CR_PLATFORM == CR_WINDOWS

}