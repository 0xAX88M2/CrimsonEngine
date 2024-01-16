#ifndef __CR_SPRITEWRITE_H__
#define __CR_SPRITEWRITE_H__

typedef enum {
	CR_ALIGN_LEFT,
	CR_ALIGN_RIGHT,
	CR_ALIGN_CENTER,
} CR_ALIGNMENT;

void cr_counter_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, int draw_number);

void cr_spritefont_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, const char *const draw_string, bool follow_camera);

unsigned int cr_get_letter_number(char product);

#endif // !__CR_COUNTER_H__
