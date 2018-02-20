
#include "backend/window.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

const u32 WINDOW_STD_WIDTH = 160;
const u32 WINDOW_STD_HEIGHT = 144;

static const u32 window_flags[4] = {
	SDL_WINDOW_SHOWN,
	SDL_WINDOW_OPENGL,
	SDL_WINDOW_FULLSCREEN,
	SDL_WINDOW_FULLSCREEN_DESKTOP
};

static const u32 render_flags[4] = {
	SDL_RENDERER_SOFTWARE,
	SDL_RENDERER_ACCELERATED,
	SDL_RENDERER_PRESENTVSYNC,
	SDL_RENDERER_TARGETTEXTURE
};

static const u32 texture_format[2] = {
	SDL_PIXELFORMAT_ARGB8888,
	SDL_PIXELFORMAT_RGBA8888
};

struct window {
	SDL_Window* win;
	SDL_Renderer *ren;
	SDL_Texture *tex;
	u32* pixel;
};

struct window*
new_window
(u32 width, u32 height, const char* name)
{
	struct window* this = malloc(sizeof(struct window));
	if (this == NULL)
		goto err_at_alloc;
	
	this->pixel = malloc(WINDOW_STD_WIDTH * WINDOW_STD_HEIGHT * sizeof(u32));
	if (this->pixel == NULL)
		goto err_at_pixel;
	
	this->win = SDL_CreateWindow(name,
	                             SDL_WINDOWPOS_CENTERED,
	                             SDL_WINDOWPOS_CENTERED,
	                             width,
	                             height,
	                             window_flags[0]);
	if (this->win == NULL)
		goto err_at_win;

	this->ren = SDL_CreateRenderer(this->win,
	                               -1,
	                               render_flags[1]);
	if (this->ren == NULL)
		goto err_at_ren;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	this->tex = SDL_CreateTexture(this->ren, 
	                              texture_format[0],
	                              SDL_TEXTUREACCESS_STREAMING,
	                              WINDOW_STD_WIDTH,
	                              WINDOW_STD_HEIGHT);
	if (this->tex == NULL)
		goto err_at_tex;

	reset_window(this);
	window_draw(this);
	return this;

err_at_tex:
	SDL_DestroyRenderer(this->ren);
err_at_ren:
	SDL_DestroyWindow(this->win);
err_at_win:
	free(this->pixel);
err_at_pixel:
	free(this);
err_at_alloc:
	return NULL;
}

void
free_window
(struct window* this)
{
	ASSERT(this);

	SDL_DestroyTexture(this->tex);
	SDL_DestroyRenderer(this->ren);
	SDL_DestroyWindow(this->win);
	free(this->pixel);
	free(this);
}

void
reset_window
(struct window* this)
{
	u32 i;
	u32 len = WINDOW_STD_WIDTH * WINDOW_STD_HEIGHT;
	for (i = 0; i < len; ++i)
		this->pixel[i] = 0x00FFFF00;	
}

void
window_draw
(struct window* this)
{
	SDL_UpdateTexture(this->tex, NULL, this->pixel, WINDOW_STD_WIDTH);
	SDL_RenderCopy(this->ren, this->tex, NULL, NULL);
	SDL_RenderPresent(this->ren);
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
