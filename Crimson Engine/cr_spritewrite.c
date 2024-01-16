#include "crimson.h"

void cr_counter_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, int draw_number)
{
	if (draw_number > 0) {
		int num, temp, factor = 1;

		num = draw_number;

		temp = num;
		while (temp) {
			temp = temp / 10;
			factor = factor * 10;
		}

		int numdigits = 0;
		if (alignment == CR_ALIGN_RIGHT) {
			temp = draw_number;
			while (temp != 0) {
				temp /= 10;
				numdigits++;
			}
		}

		int loop = 0;
		while (factor > 1) {
			factor = factor / 10;

			switch (alignment)
			{
			case CR_ALIGN_LEFT:
				cr_anim_set_frame(anim_id, num / factor);
				cr_anim_display(anim_id, pos_x + (loop * (anim_id->tileset[num / factor].width)), pos_y, CR_NO_FLIP, draw_layer);
				break;
			case CR_ALIGN_RIGHT:
				cr_anim_set_frame(anim_id, num / factor);
				cr_anim_display(anim_id, pos_x - ((numdigits - loop) * (anim_id->tileset[num / factor].width)), pos_y, CR_NO_FLIP, draw_layer);
				break;
			}

			num = num % factor;
			loop++;

		}
	}
	else {
		cr_anim_set_frame(anim_id, 0);
		cr_anim_display(anim_id, pos_x /*- anim_id->tileset[0].width*/, pos_y, CR_NO_FLIP, draw_layer);
	}
}

void cr_spritefont_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, const char *const draw_string, bool follow_cam)
{
	unsigned int string_size = strlen(draw_string);
	
	int drawpos = pos_x;
	switch (alignment)
	{
	case CR_ALIGN_LEFT:
		drawpos = pos_x;
		break;
	case CR_ALIGN_RIGHT:
		drawpos = pos_x - (anim_id->width * (string_size - 1));
		break;
	}
	
	for (unsigned int i = 0; i < string_size; i++) {
		cr_anim_set_frame(anim_id, /*cr_get_letter_number(*/draw_string[i]/*)*/);
		if (follow_cam) {
			cr_anim_display(anim_id, drawpos, pos_y, CR_NO_FLIP, draw_layer);
		}
		else {
			cr_anim_display(anim_id, drawpos + cr_camera.pos.x, pos_y + cr_camera.pos.y, CR_NO_FLIP, draw_layer);
		}
		drawpos += anim_id->width;
	}
}

unsigned int cr_get_letter_number(char product) 
{
	
	int num = product - 65;
	return num;
}