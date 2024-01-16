#ifndef CR_WIN_DRAWING_H
#define CR_WIN_DRAWING_H

void cr_windows_internal_queue_add(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short disp_x, short disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char draw_type, double rate_x, double rate_y, unsigned char par_type, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_internal_queue_draw(void);
void cr_windows_internal_queue_clear(void);

void cr_windows_internal_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short disp_x, short disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer, bool bg_cam);

//Drawing static, non-animated, single frame graphics
void cr_windows_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_sprite_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_sprite_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_sprite_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_sprite_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//You can use these to draw a section of a bitmap
void cr_windows_section_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_section_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_section_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_section_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_section_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//Draw and play an animation
void cr_windows_anim_draw(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_draw_lnscroll(CR_ANIM* anim_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_draw_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_draw_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_draw_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//Display but not play an animation
void cr_windows_anim_display(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_display_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_display_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_display_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

//Display a specific animation frame
void cr_windows_anim_display_frame(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_display_frame_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_display_frame_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_anim_display_frame_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);

void cr_windows_script_draw(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_draw_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_draw_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_draw_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

void cr_windows_script_display(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_display_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_display_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_display_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer);

void cr_windows_script_display_frame(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_display_frame_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_display_frame_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_script_display_frame_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer);

//PARALLAX FUNCTIONS
void cr_windows_parallax_draw(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_parallax_draw_lnscroll(CR_TEXTURE* sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_parallax_draw_animated(CR_ANIM *anim_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer);
void cr_windows_parallax_draw_section(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, int spr_x, int spr_y, int spr_width, int spr_height, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer);

// OTHER FUNCTIONS
void cr_windows_hires_toggle(bool enable);
void cr_windows_screen_delegate(unsigned char screen_id);
void cr_windows_screen_undelegate(void);

#endif // !CR_WIN_DRAWING_H
