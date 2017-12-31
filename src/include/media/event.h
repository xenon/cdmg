#ifndef INCLUDE_MEDIA_EVENT_H
#define INCLUDE_MEDIA_EVENT_H

	#include "debug.h"
	#include "types.h"
	
	enum event_type {
		NONE,
		P1_START,
		P1_SELECT,
		P1_U,
		P1_D,
		P1_L,
		P1_R,
		P1_A,
		P1_B,
		QUIT
	};

	extern enum event_type event_key_get(void);

	static inline bool
	event_quit
	(void)
	{
		extern bool event_quit_program;
		return event_quit_program;
	}

#endif
