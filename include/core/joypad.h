#ifndef INCLUDE_CORE_JOYPAD_H
#define INCLUDE_CORE_JOYPAD_H

#include "debug.h"
#include "types.h"

#include "backend/event.h"
#include "core/mem.h"

#define DIRECTION_WRITE ~( 0x10 | \
                           (this->joypad[3] << 3) | \
                           (this->joypad[2] << 2) | \
                           (this->joypad[1] << 1) | \
                           (this->joypad[0]))

#define BUTTON_WRITE ~( 0x20 | \
                        (this->joypad[7] << 3) | \
                        (this->joypad[6] << 2) | \
                        (this->joypad[5] << 1) | \
                        (this->joypad[4]))

static inline void
joypad_update
(struct mem* this, enum event_type key)
{
	switch (key) {
	case KEY_START:
		this->joypad[7] = !(this->joypad[7]);
		break;
	case KEY_SELECT:
		this->joypad[6] = !(this->joypad[6]);
		break;
	case KEY_B:
		this->joypad[5] = !(this->joypad[5]);
		break;
	case KEY_A:
		this->joypad[4] = !(this->joypad[4]);
		break;
 	case KEY_D:
		this->joypad[3] = !(this->joypad[3]);
		break;
	case KEY_U:
		this->joypad[2] = !(this->joypad[2]);
		break;
	case KEY_L:
		this->joypad[1] = !(this->joypad[1]);
		break;
	case KEY_R:
		this->joypad[0] = !(this->joypad[0]);
		break;
	default:
		return;
	}

	if (this->joypad_select_direction)
		this->byte[ADR_JOYPAD] = DIRECTION_WRITE;
	else
		this->byte[ADR_JOYPAD] = BUTTON_WRITE;
}

static inline void
joypad_select_button
(struct mem* this)
{
	this->joypad_select_direction = false;
	this->byte[ADR_JOYPAD] = BUTTON_WRITE;
}

static inline void
joypad_select_direction
(struct mem* this)
{
	this->joypad_select_direction = true;
	this->byte[ADR_JOYPAD] = DIRECTION_WRITE;
}

#undef DIRECTION_WRITE
#undef BUTTON_WRITE

#endif
