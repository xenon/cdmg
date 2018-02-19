#include "backend/event.h"
#include <SDL2/SDL.h>

static SDL_Event event;
static char* dropped_file;

enum event_type
event_key_get
(void)
{
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case (SDL_QUIT):
			return NOTIFY_QUIT;
	
		case (SDL_KEYDOWN):
		case (SDL_KEYUP):
			/* Tells us when a key is down or released 
			 * Simply negate the proper bit */
			switch (event.key.keysym.sym) {
			case SDLK_RETURN:
				return KEY_START;
			case SDLK_RSHIFT:
				return KEY_SELECT;
			case SDLK_UP:
				return KEY_U;
			case SDLK_DOWN:
				return KEY_D;
			case SDLK_LEFT:
				return KEY_L;
			case SDLK_RIGHT:
				return KEY_R;
			case SDLK_x:
				return KEY_A;
			case SDLK_z:
				return KEY_B;
			case SDLK_p:
				return NOTIFY_PAUSE;
			case SDLK_ESCAPE:
				return NOTIFY_QUIT;
			default:
				return NONE;
			}
		case (SDL_DROPFILE):
			dropped_file = event.drop.file;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped!", dropped_file, NULL);
			SDL_free(dropped_file);
			return NONE;

		default:
			return NONE;
		}
	} else {
		return NONE;
	}
}

