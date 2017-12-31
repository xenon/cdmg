#include "media/window.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

const u32 window_std_width = 160;
const u32 window_std_height = 144;

static const u32 window_flags[4] = {
	SDL_WINDOW_SHOWN,
	SDL_WINDOW_BORDERLESS,
	SDL_WINDOW_FULLSCREEN,
	SDL_WINDOW_FULLSCREEN_DESKTOP
};

struct window {
	SDL_Window* win;
	SDL_PixelFormat* pix;
	SDL_Renderer *ren;

	/* Add the format of copy data and
	 * Possibly add the direct copy data here */
};

struct window*
new_window
(u32 width, u32 height, u8 scale)
{
	struct window* this = malloc(sizeof(struct window));
	if (this == NULL)
		goto err_at_alloc;
	
	this->win = SDL_CreateWindow("CDMG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, scale);
	return this;

err_at_alloc:
	return NULL;
}

void
free_window
(struct window* this)
{
	
	free(this);
}

void
reset_window
(struct window* this)
{

}

void
window_draw
(struct window* this)
{

}

bool
window_fullscreen
(struct window* this)
{
	return SDL_SetWindowFullscreen(this->win, window_flags[2]);
}

bool
window_windowed
(struct window* this)
{
	return SDL_SetWindowFullscreen(this->win, 0);
}
