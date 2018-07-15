#ifndef INCLUDE_BACKEND_EVENT_H
#define INCLUDE_BACKEND_EVENT_H

#include "debug.h"
#include "types.h"

enum event_type {
	NONE,
	KEY_START,
	KEY_SELECT,
	KEY_U,
	KEY_D,
	KEY_L,
	KEY_R,
	KEY_A,
	KEY_B,
	NOTIFY_PAUSE,
	NOTIFY_QUIT,
	NOTIFY_FILE
};

extern enum event_type event_key_get(void);

#endif
