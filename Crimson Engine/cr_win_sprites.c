#include "crimson.h"
#include "cr_win_sprites.h"

CR_TEXTURE* cr_windows_sprite_load(const char *filepath, int transparent_color)
{
	SDL_Surface *sprite_surface;
	SDL_Texture *temp_sprite;

	sprite_surface = IMG_Load(filepath);

#if CR_DEBUG_SPRITES
	if (sprite_surface == NULL) {
		printf("Sprite not found!\n");
	}
#endif

	if (transparent_color != (int)NULL) {
		unsigned char r = (transparent_color >> (16)) & 0xff;
		unsigned char g = (transparent_color >> (8)) & 0xff;
		unsigned char b = transparent_color & 0xff;

		if (sprite_surface != NULL) {
			SDL_SetColorKey(sprite_surface, SDL_TRUE, SDL_MapRGB(sprite_surface->format, r, g, b));

			SDL_SetColorKey(sprite_surface, SDL_TRUE, transparent_color);
		}
	}

	if (sprite_surface == NULL) {
#if CR_DEBUG_SPRITES
		printf("No graphic found.\n");
#endif
		sprite_surface = IMG_Load("cd/nullgfx.bmp");
	}

	temp_sprite = SDL_CreateTextureFromSurface(cr_windows.renderer, sprite_surface);

	SDL_FreeSurface(sprite_surface);

#if CR_DEBUG_SPRITES
	printf("Sprite %p loaded.\n", temp_sprite);
#endif

	return (CR_TEXTURE*)temp_sprite;
}

void cr_windows_sprite_unload(CR_TEXTURE *sprite_id)
{
	SDL_Texture *tex = NULL;
	tex = (CR_TEXTURE)sprite_id;
	SDL_DestroyTexture(tex);
#if CR_DEBUG_SPRITES
	printf("Sprite %p destroyed\n", sprite_id);
#endif
}