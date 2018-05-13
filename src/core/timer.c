#include "core/timer.h"
#include "core/mem.h"

#define CLOCK_FREQUENCY 4194304

#define TIMER_FREQ0 4096
#define TIMER_FREQ1 262144
#define TIMER_FREQ2 65536
#define TIMER_FREQ3 16384

const int timer_counts[4] = {
	CLOCK_FREQUENCY / TIMER_FREQ0,
	CLOCK_FREQUENCY / TIMER_FREQ1,
	CLOCK_FREQUENCY / TIMER_FREQ2,
	CLOCK_FREQUENCY / TIMER_FREQ3
};

void
timer_step
(struct timer* this, struct mem* mem, int cycles)
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
