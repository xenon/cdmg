#include "backend/backend.h"
#include <SDL2/SDL.h>

static bool initialized = false;
static const u32 flags_sdl = SDL_INIT_EVERYTHING;

bool
init_backend
(void)
{
	if (initialized)
		return true;

	if (SDL_Init(flags_sdl) == 0)
		initialized = true;

	return initialized;
}

bool
exit_backend
(void)
{
	if (initialized) {
		initialized = false;
		SDL_Quit();
	}

	return initialized;
}
