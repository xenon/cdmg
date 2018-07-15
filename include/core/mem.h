#ifndef INCLUDE_CORE_MEM_H
#define INCLUDE_CORE_MEM_H

#include "constants.h"
#include "debug.h"	
#include "types.h"
#include "core/memfn.h"
/*#include <stdlib.h>*/

struct bootrom;
struct cart;
union conversion;

struct mem {
	struct bootrom* boot;
	struct cart* cart;
	/* u8 c_rom_bank; */
	/* for the cart possibly use virtual functions for cart reading
	 * as in use a function pointer. This simplifies MBC additions */

/* THIS SHOULD PROBABLY GO IN THE CART...?
	u8 *c_ram; 
	u8 c_ram_bank;
	size_t c_ram_size; */
	union conversion convert;
	u8 joypad[8];
	bool joypad_select_direction;

	bool dma;
	u16 dma_counter;
	u16 dma_source;
	
	u8 byte[65536]; /* 0x000-0x8000 0x8000-0xFFFF */
};

enum {
	INTERRUPT_VBLANK  = 1,
	INTERRUPT_LCDSTAT = 1 << 1,
	INTERRUPT_TIMER   = 1 << 2,
	INTERRUPT_SERIAL  = 1 << 3,
	INTERRUPT_JOYPAD  = 1 << 4
};

extern struct mem* new_mem(struct cart* cart, struct bootrom* boot);
extern void free_mem(struct mem* this);
extern void reset_mem(struct mem* this);

extern u8 mem_rb(struct mem* this, u16 address);
extern u16 mem_rw(struct mem* this, u16 address);
extern void mem_wb(struct mem* this, u16 address, u8 data);
extern void mem_ww(struct mem* this, u16 address, u16 data);

static inline void
mem_dma
(struct mem* this)
{
	if (this->dma) {
		this->byte[ADR_OAM + this->dma_counter]
			= this->byte[this->dma_source + this->dma_counter];
		++(this->dma_counter);
		if (this->dma_counter == 160)
			this->dma = false;
	}
}

static inline void
mem_interrupt_request
(struct mem* this, u8 interrupt)
{
	interrupt |= mem_rb(this, ADR_IF);
	mem_wb(this, ADR_IF, interrupt);
}

#endif
