#ifndef INCLUDE_CORE_TIMER_H
#define INCLUDE_CORE_TIMER_H

#include "debug.h"
#include "types.h"

struct mem;

struct timer {
	s32 cycles;
	s32 div;
};

extern void timer_step(struct timer* this, struct mem* mem, int cycles);

#endif
