#include "crimson.h"

CR_FONT* cr_font_load(const char* filepath, int pt_size)
{
#if CR_PLATFORM == CR_WINDOWS
	
#endif // !CR_PLATFORM == CR_WINDOWS

	return TTF_OpenFont(filepath, pt_size);

}

void cr_font_unload(CR_FONT* font)
{
	TTF_CloseFont(font);
	font = NULL;
}

void cr_font_reset(void)
{
	/*for (unsigned short i = 0; i < cr_font.draw_count; i++) {
		SDL_DestroyTexture(cr_font.textures[i]);
	}
	cr_font.draw_count = 0;*/
}

void cr_font_flush(void)
{
	cr_font.flushed = true;
}

void cr_font_draw(CR_FONT* font, float pos_x, float pos_y, const char* text, CR_COLOR4 color)
{
#if CR_PLATFORM == CR_WINDOWS

	//SDL_Color sdlcolor;
	//sdlcolor.a = color.a;
	//sdlcolor.r = color.r;
	//sdlcolor.g = color.g;
	//sdlcolor.b = color.b;

	//SDL_Color black;
	//black.a = 255;
	//black.r = 0;
	//black.g = 0;
	//black.b = 0;
	//
	//SDL_Surface* outline_surface;

	//cr_font_set_outline(font, 2);
	//outline_surface = TTF_RenderText_Blended(font, text, black);
	//cr_font.textures[cr_font.draw_count] = SDL_CreateTextureFromSurface(cr_windows.renderer, outline_surface);
	//cr_sprite_draw(cr_font.textures[cr_font.draw_count], pos_x, pos_y, CR_NO_FLIP, 7);
	//cr_font.draw_count++;

	//SDL_FreeSurface(outline_surface);

	//SDL_Surface* text_surface;

	//cr_font_set_outline(font, 0);
	//text_surface = TTF_RenderText_Blended(font, text, sdlcolor);
	//cr_font.textures[cr_font.draw_count] = SDL_CreateTextureFromSurface(cr_windows.renderer, text_surface);
	//cr_sprite_draw(cr_font.textures[cr_font.draw_count], pos_x, pos_y, CR_NO_FLIP, 7);
	//cr_font.draw_count++;

	//SDL_FreeSurface(text_surface);

	//cr_font_set_outline(font, 0);

#endif // !CR_PLATFORM == CR_WINDOWS
}

CR_TEXTURE* cr_font_render(CR_FONT* font, const char* text, CR_COLOR4 color)
{
	SDL_Color sdlcolor;
	sdlcolor.a = color.a;
	sdlcolor.r = color.r;
	sdlcolor.g = color.g;
	sdlcolor.b = color.b;

	SDL_Surface* text_surface;

	text_surface = TTF_RenderText_Blended(font, text, sdlcolor);

	CR_TEXTURE* tex = SDL_CreateTextureFromSurface(cr_windows.renderer, text_surface);

	SDL_FreeSurface(text_surface);

	return tex;
}


void cr_font_set_outline(CR_FONT* font, int outline)
{
#if CR_PLATFORM == CR_WINDOWS

	TTF_SetFontOutline(font, outline);

#endif // !CR_PLATFORM == CR_WINDOWS
}
