#include "core/timer.h"
#include "core/mem.h"
#include "constants.h"
#include <stdlib.h>

const int timer_counts[4] = {
	GB_CLOCK_FREQ / GB_TIMER_FREQ0,
	GB_CLOCK_FREQ / GB_TIMER_FREQ1,
	GB_CLOCK_FREQ / GB_TIMER_FREQ2,
	GB_CLOCK_FREQ / GB_TIMER_FREQ3
};

struct timer {
	s32 cycles;
	s32 div;
};

struct timer*
new_timer
(void)
{
	struct timer* this = malloc(sizeof(struct timer));

	if (this == NULL)
		return NULL;

	this->cycles = 0;
	this->div = 0;
	return this;
}

void
free_timer
(struct timer* this)
{
	free(this);
}

void
reset_timer
(struct timer* this)
{
	this->cycles = 0;
	this->div = 0;
}

void
timer_step
(struct timer* this, struct mem* mem, u32 cycles)
{
	/* Handle divider register */
	this->div += cycles;
	if (this->div >= 255) {
		this->div = 0;
		mem->byte[ADR_TIMER_DIV] += 1;
	}

	/* If the clock is enabled */
	if (mem->byte[ADR_TIMER_TMC] & 0x04) {
		this->cycles -= cycles;

		/* timer requires an update */
		if (this->cycles <= 0) {
			this->cycles = timer_counts[mem->byte[ADR_TIMER_TMC] & 0x03];

			/* timer is going to overflow */
			if (mem->byte[ADR_TIMER_TIMA] == 255) {
				mem->byte[ADR_TIMER_TIMA] = mem->byte[ADR_TIMER_TMA];
				mem_interrupt_request(mem, INTERRUPT_TIMER);
			} else {
				mem->byte[ADR_TIMER_TIMA] += 1;
			}
		}
	}
}
