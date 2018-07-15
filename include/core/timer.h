#ifndef INCLUDE_CORE_TIMER_H
#define INCLUDE_CORE_TIMER_H

#include "debug.h"
#include "types.h"

struct timer;
struct mem;

extern struct timer* new_timer(void);
extern void free_timer(struct timer* this);
extern void reset_timer(struct timer* this);
extern void timer_step(struct timer* this, struct mem* mem, u32 cycles);

#endif
