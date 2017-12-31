#ifndef INCLUDE_CORE_MEM_H
#define INCLUDE_CORE_MEM_H

	#include "debug.h"	
	#include "types.h"
	/*#include <stdlib.h>*/
	
	struct bootrom;
	struct cart;

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

		u8 byte[0xFFFF]; /* 0x000-0x8000 0x8000-0xFFFF */
	};

	extern struct mem* new_mem(struct cart* cart, struct bootrom* boot);
	extern void free_mem(struct mem* this);
	extern void reset_mem(struct mem* this);
	
	extern u8 mem_rb(struct mem* this, u16 address);
	extern u16 mem_rw(struct mem* this, u16 address);
	extern void mem_wb(struct mem* this, u16 address, u8 data);
	extern void mem_ww(struct mem* this, u16 address, u16 data);
#endif
