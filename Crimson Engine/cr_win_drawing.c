#include "crimson.h"
#include "cr_win_drawing.h"

CR_QUEUE_DATA cr_draw_slots[CR_DEFAULT_LAYER_COUNT][CR_DEFAULT_LAYER_LIMIT];

//local functions
void cr_windows_internal_parallax_draw(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, short disp_x, short disp_y, int spr_x, int spr_y, int spr_width, int spr_height, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer, bool bg_cam);

void cr_windows_internal_queue_add(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short disp_x, short disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char draw_type, double rate_x, double rate_y, unsigned char par_type, unsigned char flip_flag, unsigned char draw_layer)
{

	if (draw_layer < 0) {
		draw_layer = 0;
	}
	if (draw_layer >= CR_DEFAULT_LAYER_COUNT) {
		draw_layer = CR_DEFAULT_LAYER_COUNT - 1;
	}

	if (cr_draw_slot_count[draw_layer] >= CR_DEFAULT_LAYER_LIMIT - 1) {
		return;
	}

	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].sprite_data = sprite_id;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].pos_x = pos_x;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].pos_y = pos_y;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].disp_x = disp_x;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].disp_y = disp_y;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].spr_x = spr_x;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].spr_y = spr_y;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].spr_width = spr_width;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].spr_height = spr_height;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].origin_x = origin_x;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].origin_y = origin_y;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].scale_x = scale_x;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].scale_y = scale_y;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].rot_degree = rot_degree;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].draw_type = draw_type;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].rate_x = rate_x;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].rate_y = rate_y;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].par_type = par_type;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].flip_flag = flip_flag;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].draw_layer = draw_layer;
	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].screen_delegation = cr_drawing.screen_delegation;

	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].is_hd = cr_drawing.hires_enabled;

	cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].bgcam = cr_drawing.bgcam_mode;

	if (cr_drawing.blending_enabled) {
		cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].blending = true;
		cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].blend_mode = cr_drawing.blend_mode;
		cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].blend_amount = cr_drawing.blend_amount;
	}
	if (!cr_drawing.blending_enabled) {
		cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].blending = false;
		cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].blend_mode = CR_BLEND_NONE;
		cr_draw_slots[draw_layer][cr_draw_slot_count[draw_layer]].blend_amount = 255;
	}



	if (cr_draw_slot_count[draw_layer] < CR_DEFAULT_LAYER_LIMIT - 1) {
		cr_draw_slot_count[draw_layer]++;
	}

	//cr_drawing.screen_delegation = 0;
}

void cr_windows_internal_queue_draw(void)
{
	for (int l = 0; l < CR_DEFAULT_LAYER_COUNT; l++) {
		for (int s = 0; s < cr_draw_slot_count[l]; s++) {

			if (cr_draw_slots[l][s].blending) {
				SDL_SetTextureBlendMode(cr_draw_slots[l][s].sprite_data, cr_draw_slots[l][s].blend_mode);
				SDL_SetTextureAlphaMod(cr_draw_slots[l][s].sprite_data, cr_draw_slots[l][s].blend_amount);
			}
			else {
				SDL_SetTextureBlendMode(cr_draw_slots[l][s].sprite_data, CR_BLEND_ALPHA);
				SDL_SetTextureAlphaMod(cr_draw_slots[l][s].sprite_data, 255);
			}

			if (cr_debug.draw_screen) {
				cr_drawing.hires_enabled = cr_draw_slots[l][s].is_hd;
				if (cr_draw_slots[l][s].screen_delegation == 0) {
					if (cr_draw_slots[l][s].draw_type == CR_DRAWTYPE_STANDARD) {
						cr_windows_internal_sprite_draw(
							cr_draw_slots[l][s].sprite_data,
							cr_draw_slots[l][s].pos_x, cr_draw_slots[l][s].pos_y,
							cr_draw_slots[l][s].disp_x, cr_draw_slots[l][s].disp_y,
							cr_draw_slots[l][s].spr_x, cr_draw_slots[l][s].spr_y,
							cr_draw_slots[l][s].spr_width, cr_draw_slots[l][s].spr_height,
							cr_draw_slots[l][s].origin_x, cr_draw_slots[l][s].origin_y,
							cr_draw_slots[l][s].scale_x, cr_draw_slots[l][s].scale_y,
							cr_draw_slots[l][s].rot_degree,
							cr_draw_slots[l][s].flip_flag,
							cr_draw_slots[l][s].draw_layer,
							cr_draw_slots[l][s].bgcam);
					}
					
					if (cr_draw_slots[l][s].draw_type == CR_DRAWTYPE_PARALLAX) {
						cr_windows_internal_parallax_draw(
							cr_draw_slots[l][s].sprite_data,
							cr_draw_slots[l][s].pos_x, cr_draw_slots[l][s].pos_y,
							cr_draw_slots[l][s].rate_x, cr_draw_slots[l][s].rate_y,
							cr_draw_slots[l][s].disp_x, cr_draw_slots[l][s].disp_y,
							cr_draw_slots[l][s].spr_x, cr_draw_slots[l][s].spr_y,
							cr_draw_slots[l][s].spr_width, cr_draw_slots[l][s].spr_height,
							cr_draw_slots[l][s].par_type, cr_draw_slots[l][s].flip_flag,
							cr_draw_slots[l][s].draw_layer,
							cr_draw_slots[l][s].bgcam);
					}
				}
				else {
					if (cr_draw_slots[l][s].screen_delegation - 1 == cr_drawing.target_screen - 0) {
						if (cr_draw_slots[l][s].draw_type == CR_DRAWTYPE_STANDARD) {
							cr_windows_internal_sprite_draw(
								cr_draw_slots[l][s].sprite_data,
								cr_draw_slots[l][s].pos_x, cr_draw_slots[l][s].pos_y,
								cr_draw_slots[l][s].disp_x, cr_draw_slots[l][s].disp_y,
								cr_draw_slots[l][s].spr_x, cr_draw_slots[l][s].spr_y,
								cr_draw_slots[l][s].spr_width, cr_draw_slots[l][s].spr_height,
								cr_draw_slots[l][s].origin_x, cr_draw_slots[l][s].origin_y,
								cr_draw_slots[l][s].scale_x, cr_draw_slots[l][s].scale_y,
								cr_draw_slots[l][s].rot_degree,
								cr_draw_slots[l][s].flip_flag,
								cr_draw_slots[l][s].draw_layer,
								cr_draw_slots[l][s].bgcam);
						}

						if (cr_draw_slots[l][s].draw_type == CR_DRAWTYPE_PARALLAX) {
							cr_windows_internal_parallax_draw(
								cr_draw_slots[l][s].sprite_data,
								cr_draw_slots[l][s].pos_x, cr_draw_slots[l][s].pos_y,
								cr_draw_slots[l][s].rate_x, cr_draw_slots[l][s].rate_y,
								cr_draw_slots[l][s].disp_x, cr_draw_slots[l][s].disp_y,
								cr_draw_slots[l][s].spr_x, cr_draw_slots[l][s].spr_y,
								cr_draw_slots[l][s].spr_width, cr_draw_slots[l][s].spr_height,
								cr_draw_slots[l][s].par_type, cr_draw_slots[l][s].flip_flag,
								cr_draw_slots[l][s].draw_layer,
								cr_draw_slots[l][s].bgcam);
						}
					}
				}
			}
		}
	}
	cr_hires_toggle(false);
	cr_screen_undelegate();
}

void cr_windows_internal_queue_clear(void)
{
	for (int l = 0; l < CR_DEFAULT_LAYER_COUNT; l++) {
		for (int s = 0; s < cr_draw_slot_count[l]; s++) {
			cr_draw_slots[l][s].sprite_data = NULL;
			cr_draw_slots[l][s].pos_x = 0;
			cr_draw_slots[l][s].pos_y = 0;
			cr_draw_slots[l][s].disp_x = 0;
			cr_draw_slots[l][s].disp_y = 0;
			cr_draw_slots[l][s].spr_x = 0;
			cr_draw_slots[l][s].spr_y = 0;
			cr_draw_slots[l][s].spr_width = 0;
			cr_draw_slots[l][s].spr_height = 0;
			cr_draw_slots[l][s].origin_x = 0;
			cr_draw_slots[l][s].origin_y = 0;
			cr_draw_slots[l][s].scale_x = 0;
			cr_draw_slots[l][s].scale_y = 0;
			cr_draw_slots[l][s].rot_degree = 0;
			cr_draw_slots[l][s].flip_flag = 0;
			cr_draw_slots[l][s].draw_layer = 0;
			cr_draw_slots[l][s].blending = false;
			cr_draw_slots[l][s].blend_mode = CR_BLEND_ALPHA;
			cr_draw_slots[l][s].blend_amount = 255;
			cr_draw_slots[l][s].is_hd = false;
			cr_draw_slots[l][s].bgcam = false;
			cr_draw_slots[l][s].draw_type = 0;
		}
	}

	for (int i = 0; i < CR_DEFAULT_LAYER_COUNT; i++) {
		cr_draw_slot_count[i] = 0;
	}
}

void cr_windows_internal_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short disp_x, short disp_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer, bool bg_cam)
{

	if (sprite_id == NULL) {
		return;
	}

	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	vec3f* cam_pos;
	if (!bg_cam) {
		cam_pos = &cr_camera.cams[cr_drawing.target_screen]->pos;
	}
	else {
		cam_pos = &cr_camera.cams[cr_drawing.target_screen]->bg;
	}

	SDL_Rect src_rect = { ((int)pos_x - ((origin_x * (float)scale_x)) - (int)cam_pos->x) - disp_x, ((int)pos_y - (origin_y * (float)scale_y)) - ((int)cam_pos->y) - disp_y, (spr_width * (float)scale_x), (spr_height * (float)scale_y) };
	SDL_Rect dst_rect = { spr_x, spr_y, spr_width, spr_height };
	SDL_Point rot_origin = { origin_x , origin_y };

	if (cr_drawing.hires_enabled) {
		SDL_SetRenderTarget(cr_windows.renderer, cr_windows.hd_screens[cr_drawing.target_screen]);
	}
	else {
		SDL_SetRenderTarget(cr_windows.renderer, cr_windows.sd_screens[cr_drawing.target_screen]);
	}

	SDL_RenderCopyEx(cr_windows.renderer, tex, &dst_rect, &src_rect, -rot_degree, &rot_origin, flip_flag);
	crimson.draw_calls++;

}

inline void cr_windows_anim_evaluate(CR_ANIM_DATA *anim_data, CR_ANIM *anim)
{
	if (anim_data->id != anim->id) {
		// If the given animation ID is greater than the number of animations loaded...
		if (anim_data->id > anim_data->count) {
			anim_data->id = anim_data->count;				// Cap the animation ID off to the maximum
		}

		anim->prev_id = anim->id;
		anim->id = anim_data->id;						// Set the current animation ID
		anim->changed = true;

		cr_anim_add(anim_data->sheets[anim_data->scripts[anim->id]->sheet_id], anim, anim_data->scripts[anim->id]->tileset, anim_data->scripts[anim->id]->framecount, anim_data->scripts[anim->id]->speed, anim_data->scripts[anim->id]->type);
		anim->loop_frame = anim_data->scripts[anim->id]->loop_frame;
	}
	else {
		anim->prev_id = anim->id;
		anim->changed = false;
	}
}

//Drawing static, non-animated, single frame graphics
void cr_windows_sprite_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, 0, 0, 0, 0, width, height, 0, 0, 1, 1, 0, false, 0, 0, 0, flip_flag, draw_layer);
}

void cr_windows_sprite_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture* tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	switch (lnscroll_mode)
	{
	case CR_LNSCROLL_HORIZ:

		if (flip_flag == CR_FLIP_V) {
			for (int h = 0; h < height; h++) {
				cr_section_draw(sprite_id, pos_x + lnscroll_vals[h], (pos_y + height) - h, 0, h, width, 1, flip_flag, draw_layer);
			}
		}
		else {
			for (int h = 0; h < height; h++) {
				cr_section_draw(sprite_id, pos_x + lnscroll_vals[h], pos_y + h, 0, h, width, 1, flip_flag, draw_layer);
			}
		}


		break;

	case CR_LNSCROLL_VERTI:

		if (flip_flag == CR_FLIP_H) {
			for (int w = 0; w < width; w++) {
				cr_section_draw(sprite_id, (pos_x + width) - w, pos_y + lnscroll_vals[w], w, 0, 1, height, flip_flag, draw_layer);
			}
		}
		else {
			for (int w = 0; w < width; w++) {
				cr_section_draw(sprite_id, pos_x + w, pos_y + lnscroll_vals[w], w, 0, 1, height, flip_flag, draw_layer);
			}
		}

		break;
	}

}

void cr_windows_sprite_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, 0, 0, 0, 0, width, height, origin_x, origin_y, scale_x, scale_y, 0, false, 0, 0, 0, flip_flag, draw_layer);
}

void cr_windows_sprite_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, 0, 0, 0, 0, width, height, origin_x, origin_y, 1, 1, rot_degree, false, 0, 0, 0, flip_flag, draw_layer);
}

void cr_windows_sprite_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;
	 
	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, -(origin_x * scale_x) * (scale_x - 1), -(origin_y * scale_y) * (scale_y - 1), 0, 0, width, height, origin_x* scale_x, origin_y * scale_y, scale_x, scale_y, rot_degree, false, 0, 0, 0, flip_flag, draw_layer);
}

//You can use these to draw a section of a bitmap
void cr_windows_section_draw(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, 0, 0, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, false, 0, 0, 0, flip_flag, draw_layer);
}

void cr_windows_section_draw_lnscroll(CR_TEXTURE* sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture* tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	switch (lnscroll_mode)
	{
	case CR_LNSCROLL_HORIZ:

		if (flip_flag == CR_FLIP_V) {
			for (int h = 0; h < spr_height; h++) {
				cr_section_draw(sprite_id, pos_x + lnscroll_vals[h], (pos_y + spr_height) - h, spr_x, spr_y + h, spr_width, 1, flip_flag, draw_layer);
			}
		}
		else {
			for (int h = 0; h < spr_height; h++) {
				cr_section_draw(sprite_id, pos_x + lnscroll_vals[h], pos_y + h, spr_x, spr_y + h, spr_width, 1, flip_flag, draw_layer);
			}
		}

		break;

	case CR_LNSCROLL_VERTI:

		if (flip_flag == CR_FLIP_H) {
			for (int w = 0; w < spr_width; w++) {
				cr_section_draw(sprite_id, (pos_x + spr_height) - w, pos_y + lnscroll_vals[w], spr_x + w, spr_y, 1, spr_height, flip_flag, draw_layer);
			}
		}
		else {
			for (int w = 0; w < spr_width; w++) {
				cr_section_draw(sprite_id, pos_x + w, pos_y + lnscroll_vals[w], spr_x + w, spr_y, 1, spr_height, flip_flag, draw_layer);
			}
		}

		break;
	}
}

void cr_windows_section_draw_scaled(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, 0, 0, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, scale_x, scale_y, 0, false, 0, 0, 0, flip_flag, draw_layer);
}

void cr_windows_section_draw_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, 0, 0, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, 1, 1, rot_degree, false, 0, 0, 0, flip_flag, draw_layer);
}

void cr_windows_section_draw_scaled_rotated(CR_TEXTURE *sprite_id, float pos_x, float pos_y, unsigned short spr_x, unsigned short spr_y, unsigned short spr_width, unsigned short spr_height, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_internal_queue_add(sprite_id, pos_x, pos_y, 0, 0, spr_x, spr_y, spr_width, spr_height, origin_x, origin_y, scale_x, scale_y, rot_degree, false, 0, 0, 0, flip_flag, draw_layer);
}

//Draw and play an animation
void cr_windows_anim_draw(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if(!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;

		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw(anim_id->sprite, pos_x + disp_x, pos_y + disp_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, flip_flag, draw_layer);
	}

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_anim_play(anim_id);
	}
}

void cr_windows_anim_draw_lnscroll(CR_ANIM* anim_id, float pos_x, float pos_y, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;

		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		//cr_windows_section_draw(anim_id->sprite, pos_x + disp_x, pos_y + disp_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, flip_flag, draw_layer);
		cr_windows_section_draw_lnscroll(anim_id->sprite, pos_x + disp_x, pos_y + disp_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, lnscroll_vals, lnscroll_mode, flip_flag, draw_layer);
	}
	else {
		//cr_windows_sprite_draw(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, flip_flag, draw_layer);
		cr_windows_sprite_draw_lnscroll(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, lnscroll_vals, lnscroll_mode, flip_flag, draw_layer);
	}

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_anim_play(anim_id);
	}
}

void cr_windows_anim_draw_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_scaled(anim_id->sprite, pos_x, pos_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, origin_x - disp_x, origin_y - disp_y, scale_x, scale_y, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_scaled(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
	}

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_anim_play(anim_id);
	}
}

void cr_windows_anim_draw_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_rotated(anim_id->sprite, pos_x, pos_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, origin_x - disp_x, origin_y - disp_y, rot_degree, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_rotated(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
	}

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_anim_play(anim_id);
	}
}

void cr_windows_anim_draw_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_scaled_rotated(anim_id->sprite, pos_x, pos_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, origin_x - disp_x, origin_y - disp_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_scaled_rotated(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
	}

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_anim_play(anim_id);
	}
}

//Display but not play an animation
void cr_windows_anim_display(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw(anim_id->sprite, pos_x + disp_x, pos_y + disp_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, flip_flag, draw_layer);
	}
}

void cr_windows_anim_display_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_scaled(anim_id->sprite, pos_x, pos_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, origin_x - disp_x, origin_y - disp_y, scale_x, scale_y, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_scaled(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
	}
}

void cr_windows_anim_display_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
		disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_rotated(anim_id->sprite, pos_x, pos_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, origin_x - disp_x, origin_y - disp_y, rot_degree, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_rotated(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
	}
}

void cr_windows_anim_display_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
	disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
	disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_scaled_rotated(anim_id->sprite, pos_x, pos_y, anim_id->tileset[anim_id->frame_current].x, anim_id->tileset[anim_id->frame_current].y, anim_id->tileset[anim_id->frame_current].width, anim_id->tileset[anim_id->frame_current].height, origin_x - disp_x, origin_y - disp_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_scaled_rotated(anim_id->spritelist[anim_id->frame_current], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
	}
}

//Display a specific animation frame
void cr_windows_anim_display_frame(CR_ANIM *anim_id, float pos_x, float pos_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[display_frame].disp_x;
		disp_y = anim_id->tileset[display_frame].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[display_frame].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw(anim_id->sprite, pos_x + disp_x, pos_y + disp_y, anim_id->tileset[display_frame].x, anim_id->tileset[display_frame].y, anim_id->tileset[display_frame].width, anim_id->tileset[display_frame].height, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw(anim_id->spritelist[display_frame], pos_x + disp_x, pos_y + disp_y, flip_flag, draw_layer);
	}
}

void cr_windows_anim_display_frame_scaled(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[display_frame].disp_x;
		disp_y = anim_id->tileset[display_frame].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_scaled(anim_id->sprite, pos_x, pos_y, anim_id->tileset[display_frame].x, anim_id->tileset[display_frame].y, anim_id->tileset[display_frame].width, anim_id->tileset[display_frame].height, origin_x - disp_x, origin_y - disp_y, scale_x, scale_y, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_scaled(anim_id->spritelist[display_frame], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
	}
}

void cr_windows_anim_display_frame_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
		disp_x = anim_id->tileset[display_frame].disp_x;
		disp_y = anim_id->tileset[display_frame].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_rotated(anim_id->sprite, pos_x, pos_y, anim_id->tileset[display_frame].x, anim_id->tileset[display_frame].y, anim_id->tileset[display_frame].width, anim_id->tileset[display_frame].height, origin_x - disp_x, origin_y - disp_y, rot_degree, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_rotated(anim_id->spritelist[display_frame], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
	}
}

void cr_windows_anim_display_frame_scaled_rotated(CR_ANIM *anim_id, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	int disp_x = 0;
	int disp_y = 0;
	if (!anim_id->sequence) {
	disp_x = anim_id->tileset[display_frame].disp_x;
	disp_y = anim_id->tileset[display_frame].disp_y;
		if (flip_flag & CR_FLIP_H) {
			disp_x = 1 - disp_x - (anim_id->tileset[anim_id->frame_current].width);
		}
		if (flip_flag & CR_FLIP_V) {
			disp_y = -disp_y;
		}
		cr_windows_section_draw_scaled_rotated(anim_id->sprite, pos_x, pos_y, anim_id->tileset[display_frame].x, anim_id->tileset[display_frame].y, anim_id->tileset[display_frame].width, anim_id->tileset[display_frame].height, origin_x - disp_x, origin_y - disp_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
	}
	else {
		cr_windows_sprite_draw_scaled_rotated(anim_id->spritelist[display_frame], pos_x + disp_x, pos_y + disp_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
	}
}

void cr_windows_script_draw(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer) {

	cr_windows_anim_evaluate(anim_data, anim);

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_anim_draw(anim, pos_x, pos_y, flip_flag, draw_layer);	// Handle the animation as normal
	}
	else { //If anims ARE master_pause, just display it.
		cr_anim_display(anim, pos_x, pos_y, flip_flag, draw_layer);
	}
}

void cr_windows_script_draw_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_windows_anim_draw_scaled(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);	// Handle the animation as normal
	}
	else { //If anims ARE master_pause, just display it.
		cr_windows_anim_display_scaled(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);
	}
}

void cr_windows_script_draw_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_windows_anim_draw_rotated(anim, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);	// Handle the animation as normal
	}
	else { //If anims ARE master_pause, just display it.
		cr_windows_anim_display_rotated(anim, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);
	}
}

void cr_windows_script_draw_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	//If anims are not master_pause...
	if (!crimson.pause_anims) {
		cr_windows_anim_draw_scaled_rotated(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);	// Handle the animation as normal
	}
	else { //If anims ARE master_pause, just display it.
		cr_windows_anim_display_scaled_rotated(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);
	}
}

void cr_windows_script_display(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_anim_display(anim, pos_x, pos_y, flip_flag, draw_layer);	// Handle the animation as normal
}

void cr_windows_script_display_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_windows_anim_display_scaled(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, flip_flag, draw_layer);	// Handle the animation as normal
}

void cr_windows_script_display_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_windows_anim_display_rotated(anim, pos_x, pos_y, origin_x, origin_y, rot_degree, flip_flag, draw_layer);	// Handle the animation as normal
}

void cr_windows_script_display_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_windows_anim_display_scaled_rotated(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, flip_flag, draw_layer);	// Handle the animation as normal
}


void cr_windows_script_display_frame(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_windows_anim_display_frame(anim, pos_x, pos_y, display_frame, flip_flag, draw_layer);	// Handle the animation as normal
}

void cr_windows_script_display_frame_scaled(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_windows_anim_display_frame_scaled(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, display_frame, flip_flag, draw_layer);	// Handle the animation as normal
}

void cr_windows_script_display_frame_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_windows_anim_display_frame_rotated(anim, pos_x, pos_y, origin_x, origin_y, rot_degree, display_frame, flip_flag, draw_layer);	// Handle the animation as normal
}

void cr_windows_script_display_frame_scaled_rotated(CR_ANIM_DATA *anim_data, CR_ANIM *anim, float pos_x, float pos_y, short origin_x, short origin_y, float scale_x, float scale_y, float rot_degree, unsigned short display_frame, unsigned char flip_flag, unsigned char draw_layer)
{
	cr_windows_anim_evaluate(anim_data, anim);

	cr_windows_anim_display_frame_scaled_rotated(anim, pos_x, pos_y, origin_x, origin_y, scale_x, scale_y, rot_degree, display_frame, flip_flag, draw_layer);	// Handle the animation as normal
}

//PARALLAX FUNCTIONS

void cr_windows_internal_parallax_draw(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, short disp_x, short disp_y, int spr_x, int spr_y, int spr_width, int spr_height, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer, bool bg_cam)
{
	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	float true_draw_x = 0;
	float true_draw_y = 0;

	float actual_x = 0;
	float actual_y = 0;

	vec3f* cam_pos;
	if (!bg_cam) {
		cam_pos = &cr_camera.cams[cr_drawing.target_screen]->pos;
	}
	else {
		cam_pos = &cr_camera.cams[cr_drawing.target_screen]->bg;
	}

	int cam_x = (int)cam_pos->x;
	int cam_y = (int)cam_pos->y;
	true_draw_x = (float)(cam_x * rate_x) + offset_x;
	true_draw_y = (float)(cam_y * rate_y) + offset_y;

	actual_x = (true_draw_x - (int)cam_pos->x);
	actual_y = (true_draw_y - (int)cam_pos->y);

	if ((type_flag & CR_BG_H) != 0) {
		while (actual_x <= 0 - (spr_width)) {
			true_draw_x += spr_width;
			actual_x = (true_draw_x - (int)cam_pos->x);
		}
		while (actual_x > 0) {
			true_draw_x -= spr_width;
			actual_x = (true_draw_x - (int)cam_pos->x);
		}
	}

	if ((type_flag & CR_BG_V) != 0) {
		while (actual_y <= 0 - (spr_height)) {
			true_draw_y += spr_height;
			actual_y = (true_draw_y - (int)cam_pos->y);
		}
		while (actual_y > 0) {
			true_draw_y -= spr_height;
			actual_y = (true_draw_y - (int)cam_pos->y);
		}
	}

	float orig_true_draw_x = true_draw_x;
	float draw_threshold_x = actual_x + spr_width;
	float draw_threshold_y = actual_y + spr_height;

	// Looping drawing horizontally
	if (type_flag == CR_BG_H) {
		if (cr_drawing.hires_enabled) {
			while (draw_threshold_x < crimson.hd_width + spr_width) {
				cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7, bg_cam);
				true_draw_x += spr_width;
				draw_threshold_x += spr_width;
			}
		}
		else {
			while (draw_threshold_x < crimson.width + spr_width) {
				cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7, bg_cam);
				true_draw_x += spr_width;
				draw_threshold_x += spr_width;
			}
		}
	}
	else if (type_flag == CR_BG_V) { // Looping drawing vertically
		if (cr_drawing.hires_enabled) {
			while (draw_threshold_y < crimson.hd_height + spr_height) {
				cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7, bg_cam);
				true_draw_y += spr_height;
				draw_threshold_y += spr_height;
			}
		}
		else {
			while (draw_threshold_y < crimson.height + spr_height) {
				cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7, bg_cam);
				true_draw_y += spr_height;
				draw_threshold_y += spr_height;
			}
		}
	}
	else if (type_flag == (CR_BG_H | CR_BG_V)) { // Looping drawing both ways
		if (cr_drawing.hires_enabled) {
			while (draw_threshold_y < crimson.hd_height + spr_height) {

				while (draw_threshold_x < crimson.hd_width + spr_width) {
					cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7, bg_cam);
					true_draw_x += spr_width;
					draw_threshold_x += spr_width;
				}

				draw_threshold_x = actual_x + spr_width;
				true_draw_x = orig_true_draw_x;

				true_draw_y += spr_height;
				draw_threshold_y += spr_height;

			}
		}
		else {
			while (draw_threshold_y < crimson.height + spr_height) {

				while (draw_threshold_x < crimson.width + spr_width) {
					cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7, bg_cam);
					true_draw_x += spr_width;
					draw_threshold_x += spr_width;
				}

				draw_threshold_x = actual_x + spr_width;
				true_draw_x = orig_true_draw_x;

				true_draw_y += spr_height;
				draw_threshold_y += spr_height;

			}
		}
	}

	// SINGLE DRAW
	else if ((type_flag & CR_BG_NO)  == CR_BG_NO) {
		if ((type_flag & CR_BG_ONEWAY_R) == CR_BG_ONEWAY_R) {
			while (draw_threshold_x < crimson.width + spr_width) {
				//cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7);
				true_draw_x += spr_width;
				draw_threshold_x += spr_width;
			}
		}

		if ((type_flag & CR_BG_ONEWAY_L) == CR_BG_ONEWAY_L) {
			true_draw_x -= spr_width;
			draw_threshold_x -= spr_width;
			while (draw_threshold_x > 0 - spr_width) {
				//cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7);
				true_draw_x -= spr_width;
				draw_threshold_x -= spr_width;
			}
		}

		if ((type_flag & CR_BG_ONEWAY_L) != CR_BG_ONEWAY_L) {
			cr_windows_internal_sprite_draw(sprite_id, (float)true_draw_x, (float)true_draw_y, disp_x, disp_y, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, flip_flag, 7, bg_cam);
		}
	}
}

void cr_windows_parallax_draw(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	cr_internal_queue_add(sprite_id, offset_x, offset_y, 0, 0, 0, 0, width, height, 0, 0, 1, 1, 0, true, rate_x, rate_y, type_flag, flip_flag, draw_layer);
}

void cr_windows_parallax_draw_lnscroll(CR_TEXTURE* sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, signed short* lnscroll_vals, unsigned char lnscroll_mode, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture* tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	//cr_internal_queue_add(sprite_id, offset_x, offset_y, 0, 0, 0, 0, width, height, 0, 0, 1, 1, 0, true, rate_x, rate_y, type_flag, flip_flag, draw_layer);

	switch (lnscroll_mode)
	{
	case CR_LNSCROLL_HORIZ:

		//cr_parallax_draw_section(sprite_id, offset_x + lnscroll_vals[h], (offset_y + height) - h, rate_x, rate_y, 0, h, width, 1, type_flag, CR_BG_H, draw_layer);
		if (flip_flag == CR_FLIP_V) {
			for (int h = 0; h < height; h++) {
				cr_parallax_draw_section(sprite_id, offset_x + lnscroll_vals[h], (offset_y + height) - h, rate_x, rate_y, 0, h, width, 1, type_flag, flip_flag, draw_layer);
			}
		}
		else {
			for (int h = 0; h < height; h++) {
				cr_parallax_draw_section(sprite_id, offset_x + lnscroll_vals[h], offset_y + h, rate_x, rate_y, 0, h, width, 1, type_flag, flip_flag, draw_layer);
			}
		}

		break;

	case CR_LNSCROLL_VERTI:

		if (flip_flag == CR_FLIP_H) {
			for (int w = 0; w < width; w++) {
				cr_parallax_draw_section(sprite_id, (offset_x + height) - w, offset_y + lnscroll_vals[w], rate_x, rate_y, w, 0, 1, height, type_flag, flip_flag, draw_layer);
			}
		}
		else {
			for (int w = 0; w < width; w++) {
				cr_parallax_draw_section(sprite_id, offset_x + w, offset_y + lnscroll_vals[w], rate_x, rate_y, w, 0, 1, height, type_flag, flip_flag, draw_layer);
			}
		}

		break;
	}


}

void cr_windows_parallax_draw_animated(CR_ANIM *anim_id, float offset_x, float offset_y, double rate_x, double rate_y, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	unsigned short fr = anim_id->frame_current;

	SDL_Texture *tex = NULL;
	if (!anim_id->sequence) {
		tex = (CR_TEXTURE)anim_id->sprite;
	}
	else {
		tex = (CR_TEXTURE)anim_id->spritelist[fr];
	}

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	if (!anim_id->sequence) {
		cr_internal_queue_add(anim_id->sprite, offset_x, offset_y, anim_id->tileset[fr].disp_x, anim_id->tileset[fr].disp_y, anim_id->tileset[fr].x, anim_id->tileset[fr].y, anim_id->tileset[fr].width, anim_id->tileset[fr].height, 0, 0, 1, 1, 0, true, rate_x, rate_y, type_flag, flip_flag, draw_layer);
	}
	else {
		cr_internal_queue_add(anim_id->spritelist[fr], offset_x, offset_y, 0, 0, 0, 0, width, height, 0, 0, 1, 1, 0, true, rate_x, rate_y, type_flag, flip_flag, draw_layer);
	}

	cr_anim_play(anim_id);
}

void cr_windows_parallax_draw_section(CR_TEXTURE *sprite_id, float offset_x, float offset_y, double rate_x, double rate_y, int spr_x, int spr_y, int spr_width, int spr_height, unsigned char type_flag, unsigned char flip_flag, unsigned char draw_layer)
{
	int width;
	int height;

	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	cr_internal_queue_add(sprite_id, offset_x, offset_y, 0, 0, spr_x, spr_y, spr_width, spr_height, 0, 0, 1, 1, 0, true, rate_x, rate_y, type_flag, flip_flag, draw_layer);
}

//HI RES
void cr_windows_hires_toggle(bool enable)
{
	cr_drawing.hires_enabled = enable;
}

void cr_windows_screen_delegate(unsigned char screen_id)
{
	cr_drawing.screen_delegation = screen_id;
}

void cr_windows_screen_undelegate(void)
{
	cr_drawing.screen_delegation = 0;
}
