#ifndef CR_FONT_H
#define CR_FONT_H

#if CR_PLATFORM == CR_WINDOWS
typedef TTF_Font *CR_FONT;
#endif // CR_PLATFOMR == CR_WINDOWS

#define CR_FONT_DRAW_LIMIT (1024)

typedef struct {
	bool flushed;
	unsigned short draw_count;
	CR_TEXTURE* textures[CR_FONT_DRAW_LIMIT];
} CR_FONT_STRUCT;

CR_FONT_STRUCT cr_font;

CR_FONT* cr_font_load(const char* filepath, int pt_size);
void cr_font_unload(CR_FONT *font);

void cr_font_reset(void);
void cr_font_flush(void);

void cr_font_draw(CR_FONT* font, float pos_x, float pos_y, const char* text, CR_COLOR4 color);
CR_TEXTURE *cr_font_render(CR_FONT* font, const char* text, CR_COLOR4 color);

void cr_font_set_outline(CR_FONT* font, int outline);

#endif // !CR_FONT_H
