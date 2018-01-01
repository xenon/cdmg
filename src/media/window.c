#include "media/window.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

const u32 window_std_width = 160;
const u32 window_std_height = 144;

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
	u32* pixels;
};

struct window*
new_window
(u32 width, u32 height, const char* name)
{
	struct window* this = malloc(sizeof(struct window));
	if (this == NULL)
		goto err_at_alloc;
	
	this->pixels = malloc(window_std_width * window_std_height * sizeof(u32));
	if (this->pixels == NULL)
		goto err_at_pixels;
	
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
	                              window_std_width,
	                              window_std_height);
	if (this->tex == NULL)
		goto err_at_tex;
	
	return this;

err_at_tex:
	SDL_DestroyRenderer(this->ren);
err_at_ren:
	SDL_DestroyWindow(this->win);
err_at_win:
	free(this->pixels);
err_at_pixels:
	free(this);
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
