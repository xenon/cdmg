#include "core/timer.h"
#include "core/mem.h"

#define CLOCK_FREQUENCY 4194304

#define TIMER_FREQ0 4096
#define TIMER_FREQ1 262144
#define TIMER_FREQ2 65536
#define TIMER_FREQ3 16384

#define TIMER_DIV  0xFF04
#define TIMER_TIMA 0xFF05
#define TIMER_TMA  0xFF06
#define TIMER_TMC  0xFF07

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
	this->div += cycles;
	if (this->div >= 255) {
		this->div = 0;
		mem->byte[TIMER_DIV] += 1;
	}

	if (mem->byte[TIMER_TMC] & 0x04) {
		this->cycles -= cycles;
		
		if (this->cycles <= 0) {
			this->cycles = timer_counts[mem->byte[TIMER_TMC] & 0x03];
			
			if (mem->byte[TIMER_TIMA] == 255) {
				mem->byte[TIMER_TIMA] = mem->byte[TIMER_TMA];
				/* interrupt request... */
			} else {
				mem->byte[TIMER_TIMA] += 1;
			}
		}
	}
}
