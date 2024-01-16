#include "crimson.h"

int cr_sdl_handle_events()
{
	//Check for events
	while (SDL_PollEvent(&cr_sdl_events)) {

		switch (cr_sdl_events.window.event)
		{
		case SDL_WINDOWEVENT_MOVED:
			//cr_screen.window_displayindex = SDL_GetWindowDisplayIndex(cr_screen.window_id);
			//printf("Window ID: %d\n", cr_screen.window_displayindex);
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			cr_window_set_state(CR_MAXIMIZED);
			cr_screen.is_fullscreen = true;
			break;
		}

		switch (cr_sdl_events.type) {
		
		case SDL_WINDOWEVENT_CLOSE:

				if (cr_window) {
					SDL_DestroyWindow(cr_window);
					cr_window = NULL;
				}

				return 1;

				break;

		case  SDL_KEYDOWN:

			switch (cr_sdl_events.key.keysym.sym) {
				case SDLK_ESCAPE:
					//return 1;
					cr_devmenu.active = !cr_devmenu.active;
					cr_game.paused = false;
					break;
				case SDLK_F4:
					if (cr_screen.is_fullscreen) {
						cr_window_set_state(CR_WINDOWED);
						cr_screen.is_fullscreen = false;
					}
					else {
						cr_window_set_state(CR_MAXIMIZED);
						cr_screen.is_fullscreen = true;
					}
					break;
				case SDLK_F5:
					cr_stage_goto(cr_game.stage_entry);
					cr_devmenu.active = false;
					cr_game.paused = false;
					break;
				case SDLK_F6:
					cr_stage_goto(cr_stage_current);
					cr_devmenu.active = false;
					cr_game.paused = false;
					break;
				case SDLK_F7:
					cr_debug.step = true;
					break;
				case SDLK_F8:
					cr_debug.fastforward = true;
					break;

			}

			if (cr_devmenu.active) {
				switch (cr_sdl_events.key.keysym.sym) {

				}
			}

			break;

		case SDL_KEYUP:
			case SDLK_F8:
				cr_debug.fastforward = false;
				break;
			break;

		case SDL_QUIT:
			return 1;

		case SDL_JOYAXISMOTION:

			//Motion on controller 0 
			if (cr_sdl_events.jaxis.which == 0) {

				//X axis motion 
				//Left of dead zone 
				if (cr_sdl_events.jaxis.value < -p1_input.LSTICK_DEAD_ZONE) {
					p1_input.LSTICK_XDIR = -1;
				} else {
					if (cr_sdl_events.jaxis.value > p1_input.LSTICK_DEAD_ZONE) 
						p1_input.LSTICK_XDIR = 1;
					else 
						p1_input.LSTICK_XDIR = 0;
				}
				//printf("%d\n", cr_sdl_events.jaxis.value);
			}

			break;
		}
	}

	return 0;
}