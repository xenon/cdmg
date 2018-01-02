#include "media/event.h"
#include <SDL2/SDL.h>


bool event_quit_program = false;
static SDL_Event event;

/* DropEvent in SDL is a drag and drop into the window, might want support for that too */
enum event_type
event_key_get
(void)
{
	switch (event.type) {
	case SDL_QUIT:
		event_quit_program = true;
		return QUIT;

	case SDL_KEYDOWN:
	case SDL_KEYUP:
		/* Tells us when a key is down or released 
		 * Simply negate the proper bit */
		switch (event.key.keysym.sym) {
			case SDLK_RETURN:
				return P1_START;
			case SDLK_RSHIFT:
				return P1_SELECT;
			case SDLK_UP:
				return P1_U;
			case SDLK_DOWN:
				return P1_D;
			case SDLK_LEFT:
				return P1_L;
			case SDLK_RIGHT:
				return P1_R;
			case SDLK_x:
				return P1_A;
			case SDLK_z:
				return P1_B;
		}
	default:
		return NONE;
	}
}

