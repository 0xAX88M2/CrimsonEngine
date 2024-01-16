#ifndef __CR_SDL_H__
#define __CR_SDL_H__

//INCLUDES

//stdio
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

SDL_Event cr_sdl_events;

//FUNCTIONS
int cr_sdl_handle_events();

#endif // !__CR_SDL_H__