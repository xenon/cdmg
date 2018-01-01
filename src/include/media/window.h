#ifndef INCLUDE_MEDIA_WINDOW_H
#define INCLUDE_MEDIA_WINDOW_H

	#include "debug.h"
	#include "types.h"

	extern const u32 window_std_width;
	extern const u32 window_std_height;

	struct window;

	extern struct window* new_window(u32 width, u32 height, const char* name);
	extern void free_window(struct window* this);
	extern void reset_window(struct window* this);
	extern void window_draw(struct window* this);
	
	extern bool window_fullscreen(struct window* this);
	extern bool window_windowed(struct window* this);
#endif
